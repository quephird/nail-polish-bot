(ns nail-polish-bot.core
  (:require [nail-polish-bot.job :as job]))

(defn -main [& args]
  (println "Starting app...")
  (job/start-scheduler))
