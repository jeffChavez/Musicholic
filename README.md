Musicholic-v2
=============
Musicholic identifies songs being played around you and suggests an alcoholic beverage based the song's musical traits

#Usage
The user touches the circular logo button to begin listening. The app will listen for a short period of time. Upon identifying the song heard, it will display the artist, track title, and cover art, along with the "Find Drink" button. If the user touches the "Find Drink" button, the app will recommend an alchoholic beverage based on the musical traits of the song identified. For example, if listening to an energetic dance track, Musicholic may recommend a Vodka RedBull, or shots. Please drink responsibly.

![Screenshot 1] (https://github.com/jeffChavez/Musicholic-v2/blob/master/ReadMeScreenshots/IMG_0607.PNG)
![Screenshot 1] (https://github.com/jeffChavez/Musicholic-v2/blob/master/ReadMeScreenshots/IMG_0605.PNG)

#Development
The front end of this app was built using the GraceNote iOS SDK. We gutted the sample app, only keeping the features we needed, (like the audio fingerprinting technology), and rebuilt the UI from the ground up. Musicholic features a minimal and clean design, making clever use of UIView Animations, CoreImage (GaussianBlur), and CoreAnimation (Gradients).

The back end was built with Node.js, Express, and MongoDB.

#Authors
iOS
Jeff Chavez
Parker Lewis

JavaScript
Simon Kim
Joe Teo
