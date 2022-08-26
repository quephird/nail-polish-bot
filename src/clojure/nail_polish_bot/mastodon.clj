(ns nail-polish-bot.mastodon
  (:require [cheshire.core :as json]
            [clj-http.client :as http]
            [clojure.tools.logging :as log]
            [environ.core :as env]))

(defn post-to-endpoint
  "Utility function which `POST`s to the Mastodon endpoint
   correspondent with the `entity` passed in, merging the options
   passed in with the ones that are always used."
  [entity options]
  (let [instance        (env/env :mastodon-instance)
        access-key      (env/env :mastodon-access-key)
        endpoint        (format "https://%s/api/v1/%s" instance entity)
        auth-header-val (format "Bearer %s" access-key)
        options         (merge {:headers {"Authorization" auth-header-val}
                                :accept :json} options)]
    (http/post endpoint options)))

(defn post-status
  "`POST`s a message to Mastodon, and optionally with IDs of
   images previously `POST`ed."
  [status & {:keys [media-ids]}]
  (let [body      {:status status
                   :media_ids media-ids}
        body-json (json/generate-string body)]
    (post-to-endpoint "statuses"
                      {:body body-json
                       :content-type :json})))

(defn get-id-from-attachment
  "Utility function pluck the ID from the attachment response
   from a `POST` to the media endpoint."
  [attachment]
  (-> attachment
      :body
      (json/parse-string true)
      :id))

(defn post-media
  "Takes a single filename of an image and `POST`s to the
   media endpoint of Mastodon; returns an attachment object
   as the response from the endpoint."
  [filename]
  ;; Note that this always assumes a PNG file.
  (let [form-data [{:name "Content-Type" :content "image/png"}
                   {:name "file" :content (clojure.java.io/file filename)}]]
    (post-to-endpoint "media"
                      {:multipart form-data})))

(defn post-status-with-media
  "`POST`s a new status with both the message in the body of the
   toot and the images attached to it."
  [status filename]
  (try
    (log/info "Posting to Mastodon...")
    (let [media-ids [(->> filename
                          post-media
                          get-id-from-attachment)]]
      (post-status status :media-ids media-ids))
    (catch Exception e
      (log/error e "Could not post to Mastodon: "))))
