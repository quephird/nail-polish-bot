(ns nail-polish-bot.twitter
  (:require [environ.core :as env]
            [twitter.api.restful :as api]
            [twitter.oauth :as oauth]
            [twitter.request :as req]))

(defn post-status-with-media
  [status filename]
  (try
    (let [env-vars   (map env/env [:app-consumer-key
                                   :app-consumer-secret
                                   :user-access-token
                                   :user-access-token-secret])
          bot-creds  (apply oauth/make-oauth-creds env-vars)
          new-file   (req/file-body-part filename)
          new-status (req/status-body-part status)
          body       [new-file new-status]]
      (println "Posting to Twitter...")
      (api/statuses-update-with-media :oauth-creds bot-creds
                                      :body        body))
    (catch Exception e (println "Could not post to Twitter: " (.getMessage e)))))
