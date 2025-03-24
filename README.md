# Find Your Brew

Brewery search Android application with Qt Quick, QML and C++.

During the project, I took notes on my progress and my learning experience below is written using those. The development process can also be followed through my git commits, which show how mistakes were made and fixed, logic was improved, and detours were taken before the project took its final form. I first stored my code in another repository and later started moving the project to this repository, so the git history does not cover the very early stages.

---

## **Background and Starting Point**  

My programming background consists mainly of C and C++ programming, with some experience in Python, and basics of Java, JavaScript, CSS and HTML. I have no prior experience in mobile app development, so this assignment was my first time developing for Android. I was also unfamiliar with Qt, QML and Qt Creator.  

I felt quite comfortable with the C++ side of the assignment having used the language in my Hive Helsinki studies. I have also developed API clients in school projects (although in Python), so the logic of API requests and responses was familiar to me prior to this assignment. However, the Qt signal-slot system for event handling was a new concept to me.

---

## **Setup**  

To prepare for the project, I enrolled in several courses in the Qt Academy, read Qt documentation and watched some third-party video tutorials available on YouTube to get a better understanding of Qt, QML and Android development in general.  

Setting up my development environment was a learning curve in itself. I used the open-source license of Qt 6.8.2 and the Qt Creator 15.0.1 IDE. As someone new to cross-platform development, I encountered some initial installation difficulties. I had no prior knowledge on which development kits are required for targeting different platforms, including Android. Understanding which packages and components were essential for Android development within the Qt ecosystem required investigation.  

After the initial research and setup, I looked more closely at the signal-slot mechanism and made a simple test app with one button to check I understood the concept.

---

## **Creation Process**  

I started the actual assignment with the aspect most familiar to me: C++ and API client functionality. I began testing with desktop and later deployed the app to an Android virtual device and my phone (Pixel 9).  

### **C++ and Qt**  

I began with the search functionalities and created a simple UI to test them. I started with the “search for the longest name” and extended this logic for searching the northernmost and southernmost breweries.  

Later, I realized that instead of using the same URL to fetch all breweries in Ireland, I can use the “by_dist” query parameter for the northernmost and southernmost searches, which made the final solution simpler and more efficient. It also saved me from repeatedly comparing double latitude values and dealing with their possible inaccuracy when it comes to precision.  

At this point, I made basic error checks and handled basic edge cases, like several breweries having the same name length. I solved this by saving the brewery names in a list instead of a string.  

Later, I implemented pagination to the “longest name search” functionality. Currently, there are 70 breweries in Ireland, but in case the industry grows, or the app is used to fetch breweries from a larger country, I implemented response handling page by page and set `per_page` to 50 as it seems a reasonable number and is the default in the Open Brewery DB.  

Initially, I made a version that fetched the actual number of breweries through the metadata endpoint and used this to determine whether pagination was needed or not. However, I discarded this idea as going straight to pagination is simpler and covers more cases.  

### **QML**  

After getting the API response handling to work, I spent some days developing the UI.  

Initially, I was planning on using Qt Designer, but after installing and researching it, I decided I would probably not have the time to learn a new tool and instead made changes directly to the `.qml` files.  

I wanted to have several pages/views in my app and researched the different ways to transition from one view to another. I first implemented a version using `StackView` but switched to `Loader` for simplicity, since `Loader` only loads the necessary components when required, working more efficiently in a small app like this.  

I decided to create a welcome page and a search page, with a possible addition of a map page if time permits.

---

## **Working Prototype**  

At this stage of the process (19 March), I had a separate working UI and API functionalities, which I combined to make the first version of a working app.  

The merging of the two was quite easy, and encouraged by this success, I decided to try to implement a map functionality. It required some research to find out which additional libraries were required and how the Open Street Map plugin works in Qt Quick apps.  

I made a separate page/view for the map, added a marker icon, and other features such as zooming and dragging. At this stage, there was no way to pass coordinates to the map, so it was an early prototype. I decided I would look into acquiring the API key for the map resource to remove the “API Key required” watermark.

---

## **Android Deployment**  

Thus far, I had tested the app only on desktop, and the next step was to research creating emulators and deploying the app on Android.  

This was perhaps the trickiest part for me, as I had no experience in the area. Creating the virtual Android device was quite easy following tutorials, but the actual deployment was more challenging.  

Mostly, I had issues with OpenSSL libraries not being bundled with the Android build, which led to TLS failures when making API requests. After researching similar issues on Qt Forum, Stack Overflow, and YouTube, I learned how to manually package the required OpenSSL libraries with my application by modifying the Android deployment template.  

I managed to deploy the app both on the virtual device as well as my phone. There were some minor UI tweaks I needed to make, e.g. resizing some buttons and fonts, but the search functions worked nicely. However, the map was unresponsive.  

Considering the approaching time limit for finishing this project, I might need to remove the map functionality entirely, as it is only functional in the desktop version and even there the proper coordinates are not yet passed.

---

## **Final Stages**  

After successfully deploying a working app to Android, I still had time for some final tweaks and to finish writing my README.  

I made some final changes to the code to make it easier to read and so that functionalities can be added later. For example, I made the `fetchPage` function universal, so that other searches besides the “longest name search” can use it if more search functionalities are added later. Here I learned about using lambda functions with closures for handling network responses, which I have not done before.

Hive Helsinki peer evaluations have taught me to consider edge cases carefully, so I made improvements to my error handling. I checked edge cases and improved my error management to handle cases like breweries having invalid latitude or no name. I tested these by causing errors intentionally (e.g. creating empty arrays, setting invalid values).

---

## **Thoughts and Reflection**  

My goal was to make an app that can be developed further. For example, by adding find functions for searching breweries by state or type. This could perhaps be done using a dropdown menu with different search options.  

If I had more time, I would have continued working on the map feature and implemented it so that latitude and longitude are used to pinpoint the brewery.  

My prior programming knowledge was especially useful in working with the API responses in C++. Integrating Qt and its signal-slot mechanism into that was a nice new element. Also, I had a basic understanding of UI element hierarchy, parent-child relationships, and styling principles through CSS, which helped me to understand QML structure.  

The trickiest part was the deployment, as I had no prior experience of cross-platform or Android development. In hindsight, I would have started testing on Android earlier instead of focusing primarily on desktop. This would have allowed me to tackle platform-specific challenges, such as OpenSSL dependencies, sooner.  

Through this project, I gained hands-on experience with the Qt framework, its development tools, and QML, while also getting more practice in API client implementation in C++.  

I am quite happy with my final product, given my lack of experience in this area. Overall, this was a nice assignment and a great way to get into Android development and Qt.

