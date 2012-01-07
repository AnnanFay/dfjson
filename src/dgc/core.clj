(ns dgc.core
  "Description... "
  (:use clojure.xml
        clojure.pprint
        clojure.java.io))

; NOTE: This code is ugly as fuck. Don't take it as anything other than that!


;(defn get-encode [type body]
;  (let encode-func (cond (= type "vector") get-encode-vector)
;                         (= type "array")  get-encode-array)
;                         :else             get-encode-value))
;  (str "Value encode(" type " rval) {"
;    (encode-func body)
;    "return Va)lue(val);"
;    "}"))

;A comment
(defn cmt [text]
  (str "//" text "\n"))

;A type
(defn type [& all]
  (str (apply str all) "\n"))

;Value encode(df::unit::T_unknown4);
(defn encode-compound [past data]
  (let [{:keys [tag attrs content]} data]
    (str (cmt "Compound")
         (type "df::" past "::T_" (:name attrs)))))

;Value encode(df::enum_field<df::profession,int16_t>);
(defn encode-enum [data]
  (let [{:keys [tag attrs content]} data]
    (str (cmt "Enum")
         (type "df::enum_field<" (:type-name attrs) ", " (:base-type attrs) ">"))))

;Value encode(std::vector<int16_t>);
(defn encode-stl-vector [data]
  (let [{:keys [tag attrs content]} data
        vec-type (or (:type-name attrs) (:type-name (first content)))]
    (str (cmt "STL Vector")
         (type "std::vector<" (if (nil? vec-type) "void*" vec-type) ">"))))

;Value encode(std::vector<int16_t>);
(defn encode-stl-string [data]
  (let [{:keys [tag attrs content]} data]
    (str (cmt "STL String")
         (type "std::string"))))

;NOT IMPLLEMENTED YET - don't know?
(defn encode-static-array [data]
  (let [{:keys [tag attrs content]} data]
    (str (cmt "Static Array")
         (type (:name attrs) "[]"))))

(defn encode-value [data]
  (let [{:keys [tag attrs content]} data]
    (str (cmt "Value")
         (type (name tag)))))


(defn encode-struct-type-child [past data]
  (let [{:keys [tag attrs content]} data]
    (cond
      (= tag :compound)     (encode-compound past data)
      (= tag :stl-vector)   (encode-stl-vector data)
      (= tag :stl-string)   (encode-stl-string data)
      (= tag :enum)         (encode-enum data)
      (= tag :static-array) (encode-static-array data)
      (= tag :code-helper)  ""
      :else                 (encode-value data))))

;val.push_back(Pair("name", encode(rval->name)));
(defn encode-as-part-of [data]
  (let [{:keys [tag attrs content]} data
        name (:name attrs)]
    (str \tab "val.push_back(Pair(\"" name "\", encode(rval->" name ")));\n")))



; Value encode(df::unit* unit);
(defn encode-struct-type [data]
  (let [{:keys [tag attrs content]} data
    tname (:type-name attrs)]

    (str (cmt "Struct")
        ; own type
        "Value encode(" tname " rval) {\n"
        \tab "Object val;\n"
        ; get children for structure
        (apply str (map encode-as-part-of content))
        \tab "return Value(val);\n"
        "}\n"

        ; get children types
        (apply str (map (partial encode-struct-type-child tname) content)))))


(defn encode-bitfield-type [data]
  (let [{:keys [tag attrs content]} data]
    (str (cmt "Bitfield")
         (type (:type-name attrs)))))
  
(defn encode-dd-child [data]
  (let [data (assoc data :attrs (assoc (:attrs data) :type-name (str "df::" (-> data :attrs :type-name))))
        {:keys [tag attrs content]} data]
    (if (= tag :struct-type)
      (encode-struct-type data)
      (encode-bitfield-type data))))

(defn encode-dd [data]
  (let [{:keys [tag attrs content]} data]
    (apply str (map encode-dd-child content))))

(defn not-nil? [foo]
  (not (nil? foo)))

(defn get-type-name [data & [parent]]
  (let [{:keys [tag attrs content]} data
        tname (:type-name attrs)
        nname (:name attrs)
        first-child (first content)
        parent-tname (-> parent :attrs :type-name)
        parent-name (-> parent :attrs :name)]
    (cond
      (nil? data)               "void"
      (string? data)            "[string]"
      (= tag :data-definition)  (str "")
      (= tag :code-helper)      (str "")
      (= tag :comment)          (str "")
      (= tag :padding)          (str "")
      (= tag :bitfield-type)    (str "df::" tname)
      (= tag :flag-bit)         (str "") ; unsigned int, though never needed
      (= tag :struct-type)      (str "df::" tname)
      (= tag :compound)         (str "df::" (if (not-nil? content) (str parent-tname "::")) (or tname (str "T_" nname)))
      (= tag :bitfield)         (str "df::" parent-tname "::" (str "T_" nname))
      (= tag :stl-string)       (str "std::string")
      (= tag :stl-vector)       (str "std::vector<" (or tname (get-type-name first-child data)) ">")
      (= tag :static-array)     (str (or tname (get-type-name first-child)) "[]")
      (= tag :enum)             (if (nil? tname)
                                  (str "df::enum_field<" (str "T_" nname) ", " (:base-type attrs) ">")
                                  (str "df::enum_field<" tname ", " (:base-type attrs) ">"))
      (= tag :enum-item)        (str "")
      (= tag :pointer)          (cond
                                  (not (nil? tname))    (str tname "*")
                                  (> (count content) 1) (cond
                                                          (not-nil? nname) (str "T_" nname "*")
                                                          (not-nil? parent) (str "T_" parent-name "*")
                                                          :else "")
                                  :else                 (str (get-type-name first-child) "*"))
      :else                     (str (if (nil? tag) "[WTF-tag is nil]" (name tag))))))


(defn get-types [data]
  (let [{:keys [tag attrs content]} data]
    (if (nil? content)
      '()
      (concat
        (map #(get-type-name % data) content)
        (if (nil? content)
          '()
          (apply concat (map get-types content)))))))

(defn format-header [type]
  (str "Value encode(" type ");\n"))

(defn format-headers [types]
  (apply str (map format-header types)))

(defn -main []
  (let [data (parse "df.units.xml")
        log (writer "file:output.clj")
        header "encode-unit.h"
        source "encode-unit.cpp"]
    (pprint data log)
    (spit source (encode-dd data))
    (spit header (format-headers (sort (distinct (get-types data)))))))