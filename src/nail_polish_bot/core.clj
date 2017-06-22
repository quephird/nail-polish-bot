(ns nail-polish-bot.core
  (:require [me.raynes.conch.low-level :as sh]))

(defn -main []
  (let [povray-bin  "povray"
        povray-file "hello.pov"
        povray-args (format "-d +I%s +Ohello.png +W800 +H600" povray-file)
        process     (sh/proc povray-bin povray-args)
        exit        (sh/exit-code process)]
    (if (not (zero? exit))
      (println "Uh oh, something happened")
      (println "Yay! Image generated successfully"))))
