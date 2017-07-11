(ns nail-polish-bot.core
  (:require [nail-polish-bot.job :as job]))

; TODO: Need logging
(defn -main [& args]
  (job/start-scheduler))
