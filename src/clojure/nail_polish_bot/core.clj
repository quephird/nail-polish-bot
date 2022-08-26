(ns nail-polish-bot.core
  (:require [nail-polish-bot.job :as job]
            [clojure.tools.logging :as log]))

(defn -main [& args]
  (log/info "Starting app...")
  (job/start-scheduler))
