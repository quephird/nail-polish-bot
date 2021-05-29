(ns nail-polish-bot.job
  (:require [nail-polish-bot.povray :as povray]
            [nail-polish-bot.mastodon :as mastodon]
            [nail-polish-bot.twitter :as twitter]
            [clojurewerkz.quartzite.jobs :as jobs :refer [defjob]]
            [clojurewerkz.quartzite.scheduler :as scheduler]
            [clojurewerkz.quartzite.schedule.cron :as cron]
            [clojurewerkz.quartzite.triggers :as triggers]))

(defn make-status
  "Produces a status from the three parameters of the
   generated nail polish bottle."
  [[r g b] polish-type percent-full]
  (let [status-template "(R, G, B): (%.3f, %.3f, %.3f)\nPolish type: %s\nPercent full: %2.1f"
        polish-type-desc ({0 "cream" 1 "metallic"} polish-type)]
    (format status-template r g b polish-type-desc percent-full)))

; TODO: Need to generate file name and pass it into render-image and post-status
; TODO: Need to pass params as a single hashmap instead of individual one
; TODO: Need logging
(defjob PostNewImageJob [ctx]
  (let [polish-color  (vec (take 3 (repeatedly #(rand))))
        polish-type   (rand-int 2)
        percent-full  (+ 15 (rand 80))
        bottle-number (rand-int 4)
        status        (make-status polish-color polish-type percent-full)]
    (povray/render-image polish-color polish-type percent-full bottle-number)
    ;; (mastodon/post-status-with-media status "main.png")
    (twitter/post-status-with-media status "main.png")
    ))

(defn start-scheduler
  "This is the function that is responsible for starting
   and running a Quartz job to generate a new nail polish
   image and posting it to both Twitter and Mastdon."
  []
  (let [EVERY-HOUR "0 0 * * * ?"
        EVERY-FIVE-MINUTES "* /5 * * * ?"
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
                       (cron/cron-schedule EVERY-FIVE-MINUTES))))]
    (scheduler/schedule scheduler job trigger)))
