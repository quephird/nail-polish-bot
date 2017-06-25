(ns nail-polish-bot.core
  (:require [environ.core :as env]
            [me.raynes.conch.low-level :as sh]
            [twitter.api.restful :as api]
            [twitter.oauth :as oauth]
            [twitter.request :as req]
            ))

(defn post-status [image-file-name]
  (let [env-vars  (map env/env [:app-consumer-key
                                :app-consumer-secret
                                :user-access-token
                                :user-access-token-secret])
        bot-creds (apply oauth/make-oauth-creds env-vars)]
    ; TODO: Need to check env-vars to see that it actually has something
    (api/statuses-update-with-media :oauth-creds bot-creds
                                    :body [(req/file-body-part image-file-name)
                                           (req/status-body-part "My second image post!!!")])))

(defn render-image []
  (let [povray-bin  "povray"
        povray-file "hello.pov"
        povray-args (format "-d +I%s +Ohello.png +W800 +H600" povray-file)
        process     (sh/proc povray-bin povray-args)
        exit        (sh/exit-code process)]
    ; Need to make sure exit-code actually waits for proc to complete before returning
    (if (not (zero? exit))
      (println "Uh oh, something happened")
      (println "💅 Yay! Image generated successfully 💅"))))

(defn -main [])
