(ns dgc.core
  "Description... "
  (:require [clojure.zip :as z])
  (:use clojure.xml
        clojure.pprint
        clojure.java.io))

(use '[clojure.string :only (join split)])

; NOTE: This code is fugly. Don't take it as anything other than that!

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

;;;;;
;;;;; Main functions
;;;;;

(defn get-type-name [zdata zpath]
  (let [{:keys [tag attrs content parent]} zdata
        tname (:type-name attrs)
        tname (if (= tname "pointer") "void*" tname)
        nname (:name attrs)
        first-child (first content)
        parent (first zpath)
        parent-tname (-> parent :attrs :type-name)
        parent-name (-> parent :attrs :name)
        spath (join "::" (reverse (remove nil? (map #(-> % :attrs :type-name) zpath))))
        npath (join "::" (reverse (remove nil? (map #(-> % :attrs :name) zpath))))]
    (cond
      (nil? zdata)               "void"
      (string? zdata)            ""
      (= tag :data-definition)  (str "")
      (= tag :code-helper)      (str "")
      (= tag :comment)          (str "")
      (= tag :padding)          (str "")
      (= tag :bitfield-type)    (str tname)
      (= tag :flag-bit)         (str "") ; unsigned int, though never needed?
      (= tag :struct-type)      (str spath tname)
      (= tag :compound)         (if (nil? content)
                                  (str tname)
                                  (str spath "::" (or tname (str "T_" nname))))
      (= tag :bitfield)         (str spath "::" (str "T_" nname))
      (= tag :stl-string)       (str "std::string")
      (= tag :stl-vector)       (str "std::vector<" (or tname (get-type-name first-child (conj zpath zdata))) ">")
      (= tag :static-array)     (if (nil? tname)
                                  (str tname (get-type-name first-child (conj zpath zdata)) "[]")
                                  (str tname "[]"))
      (= tag :enum)             (if (nil? tname)
                                  (str "enum_field<" (str "T_" nname) ", " (:base-type attrs) ">")
                                  (str "enum_field<" tname ", " (:base-type attrs) ">"))
      (= tag :enum-item)        (str "")
      (= tag :pointer)          (cond
                                  (not (nil? tname))    (str tname "*")
                                  (> (count content) 1) (cond
                                                          (not-nil? nname) (str spath "::T_" nname "*")
                                                          (not-nil? parent) (str spath "::T_" parent-name "*")
                                                          :else "")
                                  :else                 (str (get-type-name first-child (conj zpath zdata)) "*"))
      :else                     (str (name tag)))))


; Add type to a node
(defn add-type [znode zpath]
  (if (map? znode)
    (assoc znode :type (get-type-name znode zpath))
    znode))

; Adds type key to all nodes
(defn process [zdata]
  (walk add-type zdata))

(defn format-header [type]
  (str "Value encode(" type ");\n"))

(defn format-headers [types]
  (apply str (map format-header types)))

(defn get-headers [data]
  (sort (distinct (walk-read :type (z/xml-zip data)))))


;;;;;
;;;;; Main function
;;;;; 

(defn -main []
  (let [data-file "df.units.xml"
        header    "encode-unit.h"
        source    "encode-unit.cpp"
        zdata     (z/xml-zip (parse data-file))
        log       (writer "file:df.units.clj")
        processed (process zdata)]
    (pprint (get-headers processed) log)
    ;(spit source (format-source processed))
    (spit header (format-headers (get-headers processed)))
    ))