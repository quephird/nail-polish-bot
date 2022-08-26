(ns nail-polish-bot.povray
  (:require [clojure.java.shell :as shell]
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
    (format "-d +A +R3 +L%s +L%s +L%s +I%s +O/tmp/main.png +W1280 +H1024 %s"
            povray-src-dir
            povray-fonts-dir
            povray-includes-dir
            povray-file
            user-args)))

(defn render-image [polish-color polish-type percent-full bottle-number]
  (let [povray-bin          "povray"
        povray-file         "main.pov"
        povray-includes-dir (env/env :povray-includes-dir)
        povray-args         (build-povray-args povray-includes-dir povray-file polish-color polish-type percent-full bottle-number)
        _                   (println "Rendering image...")
        result              (shell/sh povray-bin povray-args)
        exit                (:exit result)]
    (if (not (zero? exit))
      (do
        (println "Uh oh, something happened")
        (println (:err result)))
      (println "💅 Yay! Image generated successfully 💅"))))
