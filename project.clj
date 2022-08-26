(defproject nail-polish-bot "0.3.0-SNAPSHOT"
  :description "A silly little project that generates random scenes of
                nail polish bottles using POV-Ray and posts them to
                a special Twitter account"
  :url "https://github.com/quephird/nail-polish-bot"
  :license {:name "Eclipse Public License"
            :url "http://www.eclipse.org/legal/epl-v10.html"}
  :dependencies [[org.clojure/clojure "1.8.0"]
                 [cheshire "5.8.0"]
                 [clj-http "3.9.1"]
                 [clojurewerkz/quartzite "2.0.0"]
                 [com.fzakaria/slf4j-timbre "0.3.21"]
                 [environ "1.1.0"]
                 [org.clojure/tools.logging "1.2.4"]
                 [twitter-api "1.8.0"]]
  :min-lein-version "2.0.0"
  :source-paths ["src/clojure"]
  :main nail-polish-bot.core)
