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
         (type (:name attrs) "[FOO]"))))

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

(defn get-type-name [object]
  (let [{:keys [tag attrs content]} data]
    (cond
      (= tag :compound)     "df::parent::" (:type-name attrs)
      (= tag :stl-vector)   "std::vector<" (or (:type-name attrs) (get-type-name (first contents))) ">"
      (= tag :stl-string)   "std::string"
      (= tag :enum)         "df::enum_field<" (:type-name attrs) ", " (:base-type attrs) ">"
      (= tag :static-array) (get-type-name (first contents)) "[]"
      (= tag :code-helper)  ""
      :else                 (:type-name attrs))))

; Value encode(df::unit* unit);
(defn encode-struct-type [data]
  (let [{:keys [tag attrs content]} data
    type-name (:type-name attrs)]

    (str (cmt "Struct")
        ; own type
        "Value encode(" type-name " rval) {\n"
        \tab "Object val;\n"
        ; get children for structure
        (apply str (map encode-as-part-of content))
        \tab "return Value(val);\n"
        "}\n"

        ; get children types
        (apply str (map (partial encode-struct-type-child (:type-name attrs)) content)))))


(defn encode-bitfield-type [data]
  (let [{:keys [tag attrs content]} data]
    (str (cmt "Bitfield")
         (type (:type-name attrs)))))
  
(defn encode-dd-child [data]
  (let [data (assoc data :attrs (assoc (:attrs data) :type-name (str "df::" (:type-name (:attrs data)))))
        {:keys [tag attrs content]} data]
    (if (= tag :struct-type)
      (encode-struct-type data)
      (encode-bitfield-type data))))

(defn encode-dd [data]
  (let [{:keys [tag attrs content]} data]
    (apply str (map encode-dd-child content))))

(defn -main []
  (let [data (parse "df.units.xml")
        log (writer "file:output.clj")
        out "encode-unit.h"]
    (pprint data log)
    (spit out (encode-dd data))))