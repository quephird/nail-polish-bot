(ns nail-polish-bot.job
  (:require [nail-polish-bot.povray :as povray]
            [nail-polish-bot.twitter :as twitter]
            [clojurewerkz.quartzite.jobs :as jobs :refer [defjob]]
            [clojurewerkz.quartzite.scheduler :as scheduler]
            [clojurewerkz.quartzite.schedule.cron :as cron]
            [clojurewerkz.quartzite.triggers :as triggers]))

; TODO: Need to generate file name and pass it into render-image and post-status
; TODO: Need to pass params as a single hashmap instead of individual one
; TODO: Need logging
(defjob PostNewImageJob [ctx]
  (let [polish-color  (vec (take 3 (repeatedly #(rand))))
        polish-type   (rand-int 2)
        percent-full  (+ 15 (rand 80))
        bottle-number (rand-int 3)]
    (povray/render-image polish-color polish-type percent-full bottle-number)
    (twitter/post-status "main.png" polish-color polish-type percent-full bottle-number)))

(defn start-scheduler []
  (let [EVERY-HOUR "0 0 * * * ?"
        scheduler  (-> (scheduler/initialize) scheduler/start)
        job        (jobs/build
                     (jobs/of-type PostNewImageJob)
                     (jobs/with-identity (jobs/key "jobs.post-new-image")))
        trigger    (triggers/build
                     (triggers/with-identity
                       (triggers/key "triggers.post-new-image"))
                     (triggers/start-now)
                     (triggers/with-schedule
                       (cron/schedule
                       (cron/cron-schedule EVERY-HOUR))))]
    (scheduler/schedule scheduler job trigger)))
