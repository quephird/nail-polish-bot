(ns nail-polish-bot.twitter
  (:require [environ.core :as env]
            [twitter.api.restful :as api]
            [twitter.oauth :as oauth]
            [twitter.request :as req]))

; TODO: Need logging
(defn post-status [image-file-name [r g b] percent-full bottle-number]
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
