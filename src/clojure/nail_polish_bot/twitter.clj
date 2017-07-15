(ns nail-polish-bot.twitter
  (:require [environ.core :as env]
            [twitter.api.restful :as api]
            [twitter.oauth :as oauth]
            [twitter.request :as req]))

; TODO: Need logging
(defn post-status [image-file-name [r g b] polish-type percent-full bottle-number]
  (let [env-vars  (map env/env [:app-consumer-key
                                :app-consumer-secret
                                :user-access-token
                                :user-access-token-secret])
        bot-creds (apply oauth/make-oauth-creds env-vars)
        polish-type-desc ({0 "cream" 1 "metallic"} polish-type)
        status    (format "(R, G, B): (%.3f, %.3f, %.3f); polish type: %s; percent full: %2.1f" r g b polish-type-desc percent-full)]
    ; TODO: Need to check env-vars to see that it actually has something
    (api/statuses-update-with-media :oauth-creds bot-creds
                                    :body [(req/file-body-part image-file-name)
                                           (req/status-body-part status)])))
