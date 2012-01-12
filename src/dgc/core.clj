(ns dgc.core
  "Description... "
  (:require [clojure.zip :as z]
            [clojure.string :as s])
  (:use clojure.xml
        clojure.pprint
        clojure.java.io))

; NOTE: This code is fugly. Don't take it as anything other than that!

;;;;;
;;;;; Utility
;;;;;

(defn not-nil? [foo]
  (not (nil? foo)))

(defn in? 
  "true if seq contains elm"
  [elm seq]  
  (some #(= elm %) seq))

;;;;;
;;;;; Recursive walking functions
;;;;;

; Walks a tree applying f to every node with path
; f returns the new node!
(defn walk [f form]
  (loop [loc form]
    (if (z/end? loc)
      (z/root loc)
      (recur (z/next (z/replace loc (f (z/node loc) (z/path loc))))))))

; Walks a tree calling f on every node
; the results are returned as a sequence
(defn walk-read [f form]
  (loop [loc form res []]
    (if (z/end? loc)
      res
      (recur (z/next loc) (conj res (f (z/node loc)))))))

; returns first match
(defn walk-find [pred form]
  (loop [loc form]
    (cond
      (z/end? loc)        nil
      (pred (z/node loc)) loc
      :else (recur (z/next loc)))))

(defn loc-children [loc]
  (loop [child (z/down loc) children []]
    (if (nil? child)
      children
      (recur (z/right child) (conj children child)))))

;;;;;
;;;;; Render functions - output string
;;;;;

(declare render render-meta)
(def ^:dynamic headers-only false)

; returns :: separated path from df root to parent
; only ld:meta compound and struct-type is included
(defn type-path [zdata]
  (let [zpath (z/path zdata)
        zpath (filter
                #(in? (:ld:meta (:attrs %)) ["compound", "struct-type", "class-type"])
                zpath)
        zpath (map
                #(some (:attrs %) [:type-name :ld:typedef-name])
                zpath)
        zpath (remove nil? zpath)]
    (if (empty? zpath)
      "df::"
      (str "df::" (s/join "::" zpath) "::"))))


(def primitive-types ['int8_t 'uint8_t 'int16_t 'uint16_t 'int32_t 'uint32_t 'int64_t 'uint64_t 's-float 'bool 'flag-bit 'padding 'static-string])

(def primitive-type-aliases {
    's-float        'float
    'static-string  'char
    'flag-bit       'void
    'padding        'void})

(defn get-primitive-type [subtype]
  (str (get primitive-type-aliases subtype subtype)))

(defn decode_type_name_ref [&more]
  "NOT IMPLEMENTED")

(declare get-type)

; if their are children get_struct_field_type
; else void
(defn container-item-type [zdata & [void]]
  (let [znode         (z/node zdata)
        childs (:content znode)]
  (if (empty? childs)
    (or void "void*")
    (get-type (z/down zdata)))))

(defn get-container-type [zdata]
  (let [subtype (-> (z/node zdata) :attrs :ld:subtype)]
    (case subtype
      "stl-bit-vector"  "std::vector<bool>"
      "stl-vector"      (str "std::vector<" (container-item-type zdata "void*") ">")
      "df-flagarray"    (str "BitArray<" (or (decode_type_name_ref zdata '-attr_name '=> 'index-enum, '-force_type '=> 'enum-type) "int") ">"))))

(defn get-type [zdata & [weak void local]]
  (let [znode         (z/node zdata)
        attrs         (:attrs znode)
        m             (:ld:meta attrs)
        subtype       (:ld:subtype attrs)
        type-name     (or (:ld:type-name attrs) (:type-name attrs))
        typedef-name  (:ld:typedef-name attrs)
        size          (or (:size attrs) 0)
        alignment     (or (:alignment attrs) 1)
        atable        [nil "char" "short" nil "int"]
        path-to       (type-path zdata)]
    (cond
      (not-nil? typedef-name)     (str path-to typedef-name)
      (= m "number")              (get-primitive-type subtype)
      (= m "bytes")
        (if (and (not-nil? local) (nil? weak))
          (cond
            (= subtype "static-string") (str "char[" size "]")
            (= subtype "padding")       (str (nth alignment atable) "[" (/ size alignment) "]")
            :else                       (throw Exception "Foo"))
          (get-primitive-type subtype))
      (= m "global")              (str "df::" type-name)
      (= m "compound")  (cond
                          (= subtype "enum")      "enum_field<" typedef-name ", " (:base-type attrs)">"
                          (= subtype "bitfield")  typedef-name
                          :else                   typedef-name)
      (= m "pointer")             (str (container-item-type zdata) "*")
      (= m "static-array")        (str (container-item-type zdata) "[]")
      (= m "primitive")           "std::string"
      (= m "container")           (get-container-type zdata))))



(defn format-add [zdata & [deref get-symbol]]
  (let [znode   (z/node zdata)
        attrs   (:attrs znode)
        n       (or (:name attrs) (:ld:anon-name attrs))
        t       (-> znode :tag)
        m       (:ld:meta attrs)
        size    (or (:size attrs) (:count attrs))
        stype   (:ld:subtype attrs)
        path-to (type-path zdata)
        indent  (apply str (repeat (- 36 (count n)) " "))
        gsym    (or get-symbol ".")]
    (cond
      ; if it's something we don't get display nothing
      (or
          (nil? n)
          (= t :code-helper)
          (= t :virtual-methods))
          ""
      ; if it's a pointer that has children
      ;set child name to your name then format the child
      (and (= m "pointer") (> (count (:content znode)) 0))
        (format-add (z/edit (z/down zdata) #(assoc % :attrs (assoc (:attrs %) :name n))) (str deref "*") gsym)
      ;else display depending on type
      :else (str
        \tab "val.push_back(Pair(\"" n "\"," indent
        (cond
          (= stype "enum")        (str "encode<" (if (= m "compound") path-to "df::")
                                                (or
                                                  (:type-name attrs)
                                                  (:ld:typedef-name attrs))
                                                (if (:base-type attrs) (str ", " (:base-type attrs)))
                                                ">(" deref "rval" gsym n ")")
          (= stype "stl-vector")  (str "encode(" deref "rval" gsym n ")")
          (= stype "flag-bit")    (str "encode(" deref "rval" gsym "bits" gsym n ")")
          (not-nil? size)         (str "encode(" deref "rval" gsym n ")")
          :else                   (str  (if (and (= m "global") (not-nil? deref) (> (count deref) 0))
                                          (str "encode(" (subs deref 1))
                                          (str "encode(" deref))
                                        "rval" gsym n ")"))
        "));\n"))))

;;;;;
;;;;; Render meta
;;;;;

; A std::string
; manually added
(defn render-meta-primitive [zdata]
  ;"//[meta primitive]\n")
  "")

(defn render-meta-pointer [zdata]
      (str
        "//[meta pointer - rendering children]\n"
        (apply str (map render (loc-children zdata)))))

; char array i.e. char[size]
(defn render-meta-bytes [zdata]
  "//[meta bytes]\n")

; uint8_t uint16_t uint32_t
; manually added
(defn render-meta-number [zdata]
  ;"//[meta number]\n")
  "")

(defn render-meta-global [zdata]
  "//[meta global]\n")

(defn render-meta-enum-type [zdata]
  "//[meta-enum-type]\n")

(defn render-meta-struct-type [zdata]
  (let [{:keys [tag attrs content]} (z/node zdata)
        path-to (type-path zdata)
        inherit (:inherits-from attrs)
        type    (or (:type-name attrs) (:ld:typedef-name attrs))]
    (if (nil? type)
      ""
      (str 
        "//[meta struct-type]\n"
        "Object encode(" path-to type (if (= (:ld:meta attrs) "class-type") "*") " rval)"
        (if headers-only
          ";\n"
          (str
            "{\n"
            (str \tab "Object val;\n")
            (if (not-nil? inherit)
              (let [inherit-loc (walk-find
                                  #(and (= (-> % :attrs :type-name) inherit) (= (:tag %) :ld:global-type))
                                  (-> zdata z/root z/xml-zip))]
                (if (not-nil? inherit-loc)
                  (do
                    ;(prn inherit "found ----> " (map format-add (loc-children inherit-loc)))
                    (str
                      "// From inheritance (" inherit ")\n"
                      (if (= (:ld:meta attrs) "class-type")
                        (apply str (map #(format-add % "" "->") (loc-children inherit-loc)))
                        (apply str (map format-add (loc-children inherit-loc))))
                      "\n"))
                  ;(prn inherit " cannot be found! ")
                  )))
            (if (= (:ld:meta attrs) "class-type")
              (apply str (map #(format-add % "" "->") (loc-children zdata)))
              (apply str (map format-add (loc-children zdata))))
            \tab "return val;\n"
            "}\n"
            (apply str (map render (loc-children zdata)))))))))

; manual template
(defn render-meta-static-array [zdata]
  (let [{:keys [tag attrs content]} (z/node zdata)]
    ; if it has a typedef we need to print out it as a struct of the content
    (if (not-nil? (:ld:typedef-name attrs))
      (render-meta-struct-type zdata)
      (str
        "//[meta static-array - rendering children]\n"
        (apply str (map render (loc-children zdata)))))))


(defn render-meta-compound [zdata]
  (let [{:keys [tag attrs content]} (z/node zdata)
        stype (:ld:subtype attrs)]
    (str "//[meta compound - " stype "] \n"
    (if (= stype "enum")
      ""
      (render-meta-struct-type zdata)))))

;same as struct
(defn render-meta-class-type [zdata]
  (str "//[meta class-type] "
    (render-meta-struct-type zdata)))

(defn render-meta-bitfield-type [zdata]
  (str "//[meta bitfield type] "
    (render-meta-struct-type zdata)))

(defn render-meta-container [zdata]
  (str "//[meta-container]\n"
    (apply str (map render (loc-children zdata)))))


;;;;;
;;;;; Render Tag
;;;;;

(defn render-comment [zdata]
  "//[comment]\n")

(defn render-code-helper [zdata]
  "//[code helper]\n")

(defn render-virtual-methods [zdata]
  "//[virtual-methods]\n")

(defn render-ld:field [zdata]
  "//[field]\n")

(defn render-enum-item [zdata]
  "//[enum item]\n")

(defn render-ld:global-type [zdata]
  (render-meta zdata))

(defn render-ld:field [zdata]
  (render-meta zdata))

(defn render-ld:item [zdata]
  (render-meta zdata))

(defn render-ld:vmethod [zdata]
  "[vmethod]")

(defn render-ld:data-definition [zdata]
  ;(prn (apply str (map :tag (z/children zdata))))
  (apply str (map render (loc-children zdata))))

(defn render-meta [zdata]
  (in-ns 'dgc.core) ; WTF!!!!!!!
  (if (string? (z/node zdata))
    ""
    (do
      ;(prn (:tag (z/node zdata)))
      ;(prn (str "render-meta-"  (name (-> (z/node zdata) :attrs :ld:meta))))
      (let [fn-name (str "render-meta-"  (name (-> (z/node zdata) :attrs :ld:meta)))
            f       (ns-resolve *ns* (symbol fn-name))]
        (f zdata)))))

(defn render [zdata]
  (in-ns 'dgc.core) ; WTF!!!!!!!
  ;(try (z/node zdata)
  ;  (catch Exception e (prn zdata)))
  (if (string? (z/node zdata))
    ""
    (do
      ;(prn (:tag (z/node zdata)))
      ;(prn (str "render-"  (name (:tag (z/node zdata)))))
      (let [fn-name (str "render-"  (name (:tag (z/node zdata))))
            f       (ns-resolve *ns* (symbol fn-name))]
        (f zdata)))))

;;;;;
;;;;; Main function
;;;;; 

(defn -main []
  (let [data-file "codegen.out.xml"
        header    "encode-df.h"
        source    "encode-df.cpp"
        zdata     (z/xml-zip (parse data-file))
        outlog    "log-out.cpp"]
    (spit source  (str
                    (slurp "encode-df.head.cpp")
                    (render zdata)))
    (binding [headers-only true]
      (spit header  (str
                    (slurp "encode-df.head.h")
                    (render zdata)
                    (slurp "encode-df.tail.h"))))))
