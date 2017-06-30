(ns nail-polish-bot.core
  (:require [environ.core :as env]
            [me.raynes.conch.low-level :as sh]
            [clojurewerkz.quartzite.jobs :as jobs :refer [defjob]]
            [clojurewerkz.quartzite.scheduler :as scheduler]
            [clojurewerkz.quartzite.schedule.cron :as cron]
            [clojurewerkz.quartzite.triggers :as triggers]
            [twitter.api.restful :as api]
            [twitter.oauth :as oauth]
            [twitter.request :as req]))

; TODO: Figure out how to capture color, percentage full, and other
;       attributes of image and incorporate them into the status.
(defn post-status [image-file-name [r g b] percent-full]
  (let [env-vars  (map env/env [:app-consumer-key
                                :app-consumer-secret
                                :user-access-token
                                :user-access-token-secret])
        bot-creds (apply oauth/make-oauth-creds env-vars)
        status    (format "(R, G, B): (%.3f, %.3f, %.3f); percent full: %2.1f" r g b percent-full)]
    ; TODO: Need to check env-vars to see that it actually has something
    (api/statuses-update-with-media :oauth-creds bot-creds
                                    :body [(req/file-body-part image-file-name)
                                           (req/status-body-part status)])))

(defn build-povray-args [povray-includes-dir
                         povray-file
                         polish-color
                         percent-full]
  (let [user-args (->> (conj polish-color percent-full)
                    (map #(format "Declare=%s=%s" %1 %2) ["R" "G" "B" "PercentFull"])
                    (clojure.string/join " "))]
    (println user-args)
    (format "-d +Lresources +L%s +I%s +Omain.png +W800 +H600 %s" povray-includes-dir povray-file user-args)))

(defn render-image [polish-color percent-full]
  (let [povray-bin          "povray"
        povray-file         "main.pov"
        povray-includes-dir (env/env :povray-includes-dir)
        povray-args         (build-povray-args povray-includes-dir povray-file polish-color percent-full)
        process             (sh/proc povray-bin povray-args)
        exit                (sh/exit-code process)]
    ; Need to make sure exit-code actually waits for proc to complete before returning
    (if (not (zero? exit))
      (println "Uh oh, something happened")
      (println "💅 Yay! Image generated successfully 💅"))))

; TODO: Need to generate file name and pass it into render-image and post-status
(defjob PostNewImageJob [ctx]
  (let [polish-color (vec (take 3 (repeatedly #(rand))))
        percent-full (+ 15 (rand 80))]
    (render-image polish-color percent-full)
    (post-status "main.png" polish-color percent-full)))

(defn -main [& args]
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
