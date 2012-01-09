(ns dgc.core
  "Description... "
  (:require [clojure.zip :as z])
  (:use [clojure.string :only (join split)]
        clojure.xml
        clojure.pprint
        clojure.java.io))

; NOTE: This code is fugly. Don't take it as anything other than that!

;;;;;
;;;;; Utility
;;;;;

(defn not-nil? [foo]
  (not (nil? foo)))

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

(defn type-path [zdata]
  (let [zpath (z/path zdata)
        fpath (map
                #(-> % :attrs :type-name)
                zpath)
        opath (reverse
                (remove nil? fpath))]
    (if (empty? opath)
      "df::"
      (str "df::" (join "::" opath) "::"))))

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
      (= m "global")              type-name
      (= m "compound")  (cond
                          (= subtype "enum")      "enum_field<" typedef-name ", " (:base-type attrs)">"
                          (= subtype "bitfield")  typedef-name
                          :else                   typedef-name)
      (= m "pointer")             (str (container-item-type zdata) "*")
      (= m "static-array")        (str (container-item-type zdata) "[]")
      (= m "primitive")           "std::string"
      (= m "container")           (get-container-type zdata))))



(defn format-add [zdata]
  (let [znode         (z/node zdata)
        attrs (:attrs znode)
        n     (or (:name attrs) (:ld:anon-name attrs))
        t     (-> znode :tag)
        m     (:ld:meta attrs)
        size  (or (:size attrs) (:count attrs))
        stype (:ld:subtype attrs)]
    (cond
      (= t :code-helper)      ""
      (= stype "enum")        (str \tab "val.push_back(Pair(\"" n "\"," \tab
                                "encode_enum<df::" (:type-name attrs) ", " (:base-type attrs) ">(rval." n ")));\n")
      (= stype "stl-vector")  (str \tab "val.push_back(Pair(\"" n "\"," \tab
                                "encode_vector<" (container-item-type zdata) ">(rval." n ")));\n")
      (not-nil? size)         (str \tab "val.push_back(Pair(\"" n "\"," \tab
                                "encode_array<" (container-item-type zdata) ">(rval." n ", " size ")));\n")
      :else                   (str \tab "val.push_back(Pair(\"" n "\"," \tab "encode(rval." n ")));\n"))))

;;;;;
;;;;; Render meta
;;;;;

; A std::string
; manually added
(defn render-meta-primitive [zdata]
  ;"//[meta primitive]\n")
  "")

(defn render-meta-pointer [zdata]
  "//[meta primitive]\n")

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

(defn render-meta-container [zdata]
  "//[meta-container]\n")

(defn render-meta-enum-type [zdata]
  "//[meta-enum-type]\n")

; manual template
(defn render-meta-static-array [zdata]
  ;"//[meta-static-array]\n")
  "")

(defn render-meta-bitfield-type [zdata]
  "//[meta bitfield-type]\n")

(defn render-meta-struct-type [zdata]
  (let [{:keys [tag attrs content]} (z/node zdata)
        path-to (type-path zdata)
        inherit (:inherits-from attrs)]
    (str 
      "//[meta (struct/class)-type]\n"
      "Value _encode(" path-to (or (:type-name attrs) (:ld:typedef-name attrs)) " rval)"
      (if headers-only
        ";\n"
        (str
          "{\n"
          \tab "Object val;\n"
          (if (not-nil? inherit)
            (str \tab "val = encode((" inherit ")rval);\n"))
          (apply str (map format-add (loc-children zdata)))
          \tab "return Value(val);\n"
          "}\n"
          (apply str (map render (loc-children zdata))))))))

(defn render-meta-compound [zdata]
  (str "//[meta compound] "
  (render-meta-struct-type zdata)))

;same as struct
(defn render-meta-class-type [zdata]
  (str "//[meta class-type] "
  (render-meta-struct-type zdata)))

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
  "[item]\n")

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
        log       (writer "file:log.clj")
        outlog    "log-out.cpp"]
    (spit source (str (slurp "encode-df.inc.cpp") (render zdata)))
    (binding [headers-only true]
      (spit header (str (slurp "encode-df.inc.h") (render zdata))))))