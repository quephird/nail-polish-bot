## Purpose

This is not a typical README in that I won't elaborate on how to run this code on your machine.
I had to learn _many_ things for this small project,
and so I felt it was much more useful to share the story of how I figured out how to put this bot together from start to finish.

## Project goals

There were multiple things that I wanted to be able to accomplish with this little project:

* Post images of nail polish bottles to Twitter (obviously)
* Somehow employ POV-Ray to render such scenes
* Somehow randomize certain aspects of the scene, such as the color of the bottle,
  and incorporate that information into the body of the tweet
* Use free hosting that supported Clojure
* Be able to run this bot on _any_ PaaS and not be reliant on any vendor-specific features

## Discussion

#### Where to host this?

I decided on Heroku because I've used them before since they're one of the few PaaS' that supports Clojure and has a free offering.
Since this bot only does anything once per hour I didn't need anything that supported much memory, CPU, or HTTP traffic.

#### Beginning the project

Installing the Heroku CLI was trivial using `brew install heroku`.
Just to get things starting, I created a new Clojure project with Leiningen and `git init`ed it.
I also needed to add `heroku` as a Git remote by running:

```
heroku git:remote -a nail-polish-bot
```

I also needed to include the `heroku/clojure` buildpack to make the JDK and Leiningen available to the project.
I did this through the Heroku Web UI in the Settings tab.

#### Installing POV-Ray on a dyno

But before I decided to invest too much effort into Heroku, I needed to figure out if I could somehow install POV-Ray onto the VM.
Heroku doesn't give you `sudo` nor `apt-get` so the chance of installing third-party libraries looked bleak.
That is, until I found `heroku-buildpack-apt` which allows you to indirectly install `apt` packages.
To add the buildpack to my dyno, I needed to run the following:

```
* Run heroku buildpacks:add --index 1 https://github.com/heroku/heroku-buildpack-apt
```

All I needed to do was create a new file at the project root called `Aptfile` and add desired packages to install on each line;
in this case I wanted to install POV-Ray, so I just needed this in the file:

```
povray
```

I could see that POV-Ray was indeed being installed by committing this one file and then deploying it to the dyno via `git push heroku master`.
The log to the screen showed (among other things) the following:

```
remote: -----> Fetching .debs for povray
remote:        Reading package lists...
remote:        Building dependency tree...
remote:        The following additional packages will be installed:
remote:          libasound2 libasound2-data libasyncns0 libboost-system1.58.0
remote:          libboost-thread1.58.0 libcaca0 libflac8 libjson-c2 libogg0 libpulse0
remote:          libsdl1.2debian libslang2 libsndfile1 libvorbis0a libvorbisenc2
remote:          povray-includes
remote:        Suggested packages:
remote:          libasound2-plugins alsa-utils pulseaudio povray-doc povray-examples
remote:        The following NEW packages will be installed:
remote:          libasound2 libasound2-data libasyncns0 libboost-system1.58.0
remote:          libboost-thread1.58.0 libcaca0 libflac8 libjson-c2 libogg0 libpulse0
remote:          libsdl1.2debian libslang2 libsndfile1 libvorbis0a libvorbisenc2 povray
remote:          povray-includes
remote:        0 upgraded, 17 newly installed, 0 to remove and 129 not upgraded.
remote:        Need to get 0 B/3,534 kB of archives.
remote:        After this operation, 13.0 MB of additional disk space will be used.
remote:        Download complete and in download only mode
remote: W: --force-yes is deprecated, use one of the options starting with --allow instead.
remote: -----> Installing libasound2_1.1.0-0ubuntu1_amd64.deb
remote: -----> Installing libasound2-data_1.1.0-0ubuntu1_all.deb
remote: -----> Installing libasyncns0_0.8-5build1_amd64.deb
remote: -----> Installing libboost-system1.58.0_1.58.0+dfsg-5ubuntu3.1_amd64.deb
remote: -----> Installing libboost-thread1.58.0_1.58.0+dfsg-5ubuntu3.1_amd64.deb
remote: -----> Installing libcaca0_0.99.beta19-2build2~gcc5.2_amd64.deb
remote: -----> Installing libflac8_1.3.1-4_amd64.deb
remote: -----> Installing libjson-c2_0.11-4ubuntu2_amd64.deb
remote: -----> Installing libogg0_1.3.2-1_amd64.deb
remote: -----> Installing libpulse0_1%3a8.0-0ubuntu3.2_amd64.deb
remote: -----> Installing libpulse0_1%3a8.0-0ubuntu3.3_amd64.deb
remote: -----> Installing libsdl1.2debian_1.2.15+dfsg1-3_amd64.deb
remote: -----> Installing libslang2_2.3.0-2ubuntu1_amd64.deb
remote: -----> Installing libsndfile1_1.0.25-10ubuntu0.16.04.1_amd64.deb
remote: -----> Installing libvorbis0a_1.3.5-3_amd64.deb
remote: -----> Installing libvorbisenc2_1.3.5-3_amd64.deb
remote: -----> Installing povray_1%3a3.7.0.0-8build1_amd64.deb
remote: -----> Installing povray-includes_1%3a3.7.0.0-8build1_all.deb
```

#### How to run POV-Ray from the command line locally

Since there is no POV-Ray library that I could import into Clojure like it's possible to do with Processing/quil,
such that I could either render scenes within Clojure,
I had no choice but to invoke the executable from with Clojure.
But before I bothered to do that,
I needed to figure out how to compose all the proper arguments to pass to POV-Ray on the command line to produce an image.

The first thing I did was to run things locally on my MBP;
I created a minimal `.pov` file and issued the following:

```
povray -d +Ihello.pov +Ohello.png +W800 +H600
```

... where the switches do the following:

* `-d` Turns off the image display after rendering
* `+I` Specifies the input file name
* `+O` Specifies the output file name
* `+W` Specifies the image width
* `+H` Specifies the image height

... and that worked like a charm.

#### How to run POV-Ray from the command line on the dyno

Next was figuring out how to test this out on the dyno... but I had no idea how to get a command prompt.
Conveniently, the Heroku CLI lets you do this by running:

```
heroku run /bin/bash
```

Awesome! But when I tried running POV-Ray the same way as above I got errors about not being able to find libraries.
To first get me by, I set LD_LIBRARY_PATH manually like this:

```
LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/app/.apt/usr/lib/x86_64-linux-gnu/pulseaudio/:/app/.apt/lib/x86_64-linux-gnu/
```
... and that worked! But this was not the ideal way to do this since every time I deployed a new version of code,
I'd get a fresh environment.
It turns out that the Heroku CLI allows you to set environment variables too:

```
heroku config:add LD_LIBRARY_PATH=/app/.apt/usr/lib/x86_64-linux-gnu/pulseaudio/:/app/.apt/lib/x86_64-linux-gnu/
```

I would later learn that you can create and modify them in the Web UI for the dyno where they are called Config Vars.

#### How to invoke POV-Ray from within Clojure

The next thing I needed to figure out was the best way to run a shell command or invoke an executable from within a Clojure program.
I did see that Clojure comes bundled with `clojure.java.shell/sh` and I was quickly able to put something together.
_But_, `clojure.java.shell/sh` doesn’t expose anything to kill the resultant process once the command completes, and so just calling `sh` just hangs the JVM.
After some googling about this issue, I came across the `me-raynes/conch` library.
I invoked POV-Ray via `me.raynes.conch.low-level/proc` and then used `me.raynes.conch.low-level/exit-code` to wait for it to complete and then return control back to the caller. Very nice.

#### Spinning up a remote REPL

The Heroku CLI also provides a means of directly starting a REPL on the remote dyno but from your local machine:

```
heroku run lein repl
```

When I tried doing so the first time, I was getting a _very_ strange error:

```
rlwrap: error while loading shared libraries: libreadline.so.5: cannot open shared object file: No such file or directory
```

I also noticed that during deployment I kept getting a warning that it was defaulting to an old version of Leiningen, and being recommended to specify a newer one by adding a `:min-lein-version` key to `project.clj`.
Since Leiningen is the only thing that I could think of that could possibly need `rlwrap` or `readline`, I figured that I needed to bump its version to at least 2.0.0.
Indeed, after doing that, the error went away and I was able to run the remote REPL.

#### Creating a new Twitter account for the bot

Creating a new Twitter account for bot, @nailpolishbot, was trivial.
There was one annoyance in that there is a restriction of only one Twitter account per email address.
I used a trick by appending `+nailpolishbot` to the email address.
That way I 1) got to create a new account and 2) emails should still get routed to the same email address; the + is just a tag.
In order to use its credentials to call the APIs, I also needed to create a new application associated with it.
Once I created the application, I then copied the four secrets to my local machine to being experimentation with the API.

#### Tweeting from Clojure

I was hoping for an easy-to-use library for using the Twitter API and I discovered `adamwynne/twitter-api`, and that turned out to be incredibly easy and understandable to use.
I was quickly able to use the library with my secrets in the REPL to post test tweets.
But I did not want to hardcode any of the four secrets that the bot needed for the Twitter API.
To avoid that, I decided to try using environment variables to store them.
It turns out that the Heroku console and CLI both allow for setting of environment variables; [this nice article](https://devcenter.heroku.com/articles/config-vars) discusses how.
I also found a Clojure library, `weavejester/environ`, that can be used to easily retrieve them from the OS.
To retrieve a value, say for `FOO_VAR`, I simply needed to refer to the keywordized, dashified, and lowercased version of the name:

```
(environ.core/env :foo-var)
```

And so, I set up my credentials in my Clojure code thusly:

```
(let [env-vars  (map env/env [:app-consumer-key
                              :app-consumer-secret
                              :user-access-token
                              :user-access-token-secret])
      bot-creds (apply oauth/make-oauth-creds env-vars)
```

... and tweeting an image instead of a simple text message turned out to be trivial using the library as well:

```
  (api/statuses-update-with-media
    :oauth-creds bot-creds
    :body [(api/file-body-part image-file-name)
           (api/status-body-part “Some status message!“)])))
```

#### Insuring POV-Ray can find all resources

Once I included POV-Ray source code for a minimal scene and tried to render the image from Clojure, I was encountering multiple errors.
The first problem is that the `povray` command needs to know where all the project scene, includes, and fonts are.
To do that, I needed to use the `+L` command line parameter to specify where to look for all of the input files, which I hardcoded into the program.
It also turned out that I also needed to specify the location of the include files that come bundled with POV-Ray, such as those for colors, textures, and shapes.
For that, I decided to use an environment variable, also managed as a Heroku config var, rather than hard code it into the Clojure code.
And that all worked!

#### Setting up a scheduled job

I wanted my bot to tweet once per hour so I needed to figure out what to use for a job scheduler.
Heroku does offer their scheduler, https://devcenter.heroku.com/articles/scheduler, but I wanted to minimize the number of external dependencies or vendor-specific solutions.
I decided to go with `michaelklishin/quartzite` since that has worked pretty well in other projects I've been involved in.
I just needed to create a job, a trigger with a simple cron expression (fir the purposes of testing once per minute), and a scheduler wired up to start in `-main`.
Running `lein run`, I was able to see tweets being posted every minute from my bot!

#### Insuring `-main` is invoked on dyno

After deploying this latest version of my bot, I saw that nothing was being tweeted.
I wanted to take a look at the application log file on the dyno; the Heroku CLI provides an easy means of viewing it just by running `heroku logs`.
By default, Heroku assumes a web app is being deployed which explains why I kept seeing log entires like this:

```
2017-07-09T18:11:47.341027+00:00 app[web.1]: Error R10 (Boot timeout) -> Web process failed to bind to $PORT within 60 seconds of launch
2017-07-09T18:11:47.436549+00:00 app[web.1]: Stopping process with SIGKILL
2017-07-09T18:11:47.513171+00:00 app[web.1]: Process exited with status 137
2017-07-09T18:11:48.634523+00:00 app[web.1]: State changed from starting to crashed
2017-07-09T18:11:50.991928+00:00 app[web.1]: State changed from crashed to starting
2017-07-09T18:11:50.991939+00:00 app[web.1]: Starting process with command 'lein trampoline run'
```

The reason why `-main` wasn’t being invoked was because I needed to add a `Procfile` and specify a _worker_ to invoke `lein run`.
This file also needs to be at the root of the project directory and simply needed to contain this:

```
worker: lein run
```

Alas, even after deploying the new version of my bot with a `Procfile`, `-main` still wasn’t being invoked.
It turns out that you have to also configure the dyno in addition to the app itself for everything to work properly.
This can also be configured in the UI but I couldn’t get the worker setting to stick and so I just ran the following on the CLI:

```
heroku ps:scale worker=1
```

## Useful links

These are but some of the important resources that I used for this project.

Leiningen  
[https://github.com/technomancy/leiningen](https://github.com/technomancy/leiningen)

heroku-buildpack-apt  
[https://elements.heroku.com/buildpacks/heroku/heroku-buildpack-apt](https://elements.heroku.com/buildpacks/heroku/heroku-buildpack-apt)

POV-Ray 3.7 documentation  
[http://www.povray.org/documentation/3.7.0/](http://www.povray.org/documentation/3.7.0/)

POV-Ray man page  
[https://www.mankier.com/1/povray](https://www.mankier.com/1/povray)

conch  
[https://github.com/Raynes/conch](https://github.com/Raynes/conch)

environ  
[https://github.com/weavejester/environ](https://github.com/weavejester/environ)

twitter-api  
[https://github.com/adamwynne/twitter-api](https://github.com/adamwynne/twitter-api)

quartzite  
[https://github.com/michaelklishin/quartzite](https://github.com/michaelklishin/quartzite)

## License

Copyright (C) 2017, ⅅ₳ℕⅈⅇℒℒⅇ Ҝⅇℱℱoℜⅆ.

Distributed under the Eclipse Public License either version 1.0 or (at your option) any later version.
