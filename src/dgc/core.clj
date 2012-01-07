(ns dgc.core
  "Description... "
  (:use clojure.xml))
(spit "output" (parse "/home/sai/df/dfhack/library/xml/df.units.xml"))