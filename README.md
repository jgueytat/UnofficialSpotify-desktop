
# Unofficial Spotify for desktop

The projet is to help developing [UnofficialSpotify for Sailfish OS](https://github.com/jgueytat/UnofficialSpotify). It's using Qt 5.9.4 with the networkauth and webengine modules. The final goal is to have a native Spotify application for Sailfish OS.

## Status

 * OAuth ok with QtNetworkAuth module
 * Web API ok
 * Web Playback SDK ok with a recompiled QtWebEngine module
 
 > Spotify is ready to go for Qt / C++ development! ;)

## Short story

Spotify Playback SDK needs the [encrypted media](https://developer.mozilla.org/fr/docs/Web/API/Encrypted_Media_Extensions_API) to be enabled in your browser. [Visit this page](https://bitmovin.com/mpeg-dash-hls-drm-test-player/) to check what's your current capabilities.

Encrypted media functionality is supported in Qt as long as you recompile QtWebEngine. You need to add the option WEBENGINE_CONFIG+=use_proprietary_codecs for the compilation. WebEngine is based on Chromium and depending on the Chromium version the [Widevine Content Decryption Module](https://www.widevine.com/) is directly integrated or not.

 * [Check](https://doc.qt.io/qt-5/qtwebengine-overview.html#qt-webengine-core-module) the version of the Chromium version integrated in QtWebEngine 5.x.
 * [Download](https://www.slimjet.com/chrome/google-chrome-old-version.php) the exact same version of Google Chrome.
 * Extract the following files:
   * widevinecdm.dll
   * widevinecdmadapter.dll
 * Place them close to your executable.

QtWebEngine 5.9.4 is based on Chromium version 56.0.2924.122. Some release after they directly integrated WideVine in Chromium and they might provide a way to disable it in a future release.
