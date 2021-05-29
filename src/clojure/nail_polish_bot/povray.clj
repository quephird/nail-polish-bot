(ns nail-polish-bot.povray
  (:require [me.raynes.conch.low-level :as sh]
            [environ.core :as env]))

(defn build-povray-args [povray-includes-dir
                         povray-file
                         polish-color
                         polish-type
                         percent-full
                         bottle-number]
  (let [povray-src-dir   "src/povray"
        povray-fonts-dir "src/povray/fonts"
        user-args        (->> (conj polish-color polish-type percent-full bottle-number)
                           (map #(format "Declare=%s=%s" %1 %2) ["R" "G" "B" "PolishType" "PercentFull" "BottleNumber"])
                           (clojure.string/join " "))]
    ; This command arg list represents the following options:
    ;
    ; -d Turns off the image display after rendering
    ; +A Turns on antialiasing
    ; +R3 Specifies the antialiasing depth to 3 on a scale of 1-9
    ; +L Specifies the directories to look for include and project files
    ; +I Specifies the input file name
    ; +O Specifies the output file name
    ; +W Specifies the image width
    ; +H Specifies the image height
    (format "-d +A +R3 +L%s +L%s +L%s +I%s +Omain.png +W1280 +H1024 %s"
            povray-src-dir
            povray-fonts-dir
            povray-includes-dir
            povray-file
            user-args)))

; TODO: Need logging
(defn render-image [polish-color polish-type percent-full bottle-number]
  (let [povray-bin          "povray"
        povray-file         "main.pov"
        povray-includes-dir (env/env :povray-includes-dir)
        povray-args         (build-povray-args povray-includes-dir povray-file polish-color polish-type percent-full bottle-number)
        process             (sh/proc povray-bin povray-args)
        exit                (sh/exit-code process)]
    ; Need to make sure exit-code actually waits for proc to complete before returning
    (println process)
    (if (not (zero? exit))
      (println "Uh oh, something happened")
      (println "💅 Yay! Image generated successfully 💅"))))
