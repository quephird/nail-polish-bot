(ns nail-polish-bot.twitter
  (:require [environ.core :as env]
            [twitter.api.restful :as api]
            [twitter.oauth :as oauth]
            [twitter.request :as req]))

; TODO: Need logging
(defn post-status-with-media
  [status filename]
  (let [env-vars   (map env/env [:app-consumer-key
                                 :app-consumer-secret
                                 :user-access-token
                                 :user-access-token-secret])
        bot-creds  (apply oauth/make-oauth-creds env-vars)
        ;; new-file   (req/file-body-part filename)
        ;; new-status (req/status-body-part status)
        ;; body       [new-file new-status]
        ;; api-params {:oauth-creds bot-creds
        ;;             :body        body}
        ]
    ; TODO: Need to check env-vars to see that it actually has something
    (try
      (api/statuses-update-with-media :oauth-creds bot-creds
                                      :body [(req/file-body-part filename)
                                             (req/status-body-part status)])
      (catch Exception e (println (.getMessage e))))))
