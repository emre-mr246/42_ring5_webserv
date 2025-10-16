#!/usr/bin/python3

import random

print("Content-Type: text/html; charset=utf-8\n")

quotes = [
    {"text": "It takes courage to stand up to enemies, but even more to stand up to friends.", "author": "A wise headmaster"},
    {"text": "The truth is powerful and it will prevail.", "author": "Ancient wisdom"},
    {"text": "Working hard is important, but believing in yourself matters even more.", "author": "A wise teacher"},
    {"text": "We are only as strong as we are united.", "author": "The Headmaster"},
    {"text": "Words are our most inexhaustible source of magic.", "author": "Albus Dumbledore"},
    {"text": "The ones we love never truly leave us.", "author": "A protective godfather"},
    {"text": "It is our choices that show what we truly are.", "author": "The wise professor"},
    {"text": "Understanding is the first step to acceptance.", "author": "A caring teacher"},
    {"text": "Fear of a name only increases fear of the thing itself.", "author": "Hermione Granger"},
    {"text": "Differences unite us when our goals are the same.", "author": "The Headmaster"},
    {"text": "Youth cannot know how age thinks and feels.", "author": "Ancient wisdom"},
    {"text": "Greatness inspires envy, which breeds spite.", "author": "A dark wizard"},
    {"text": "There are things worth more than cleverness.", "author": "Hermione Granger"},
    {"text": "Age forgets what youth knows.", "author": "A wise wizard"},
    {"text": "Do not pity those who have passed, but those still suffering.", "author": "Wise counsel"}
]

quote = random.choice(quotes)

print("<!DOCTYPE html>")
print("<html>")
print("<head>")
print("    <title>Hogwarts School of Witchcraft and Wizardry</title>")
print("    <link rel=\"stylesheet\" href=\"style.css\">")
print("</head>")
print("<body>")
print("    <div class=\"header\">")
print("        <h1>Hogwarts School of Witchcraft and Wizardry</h1>")
print("        <p class=\"motto\">Draco Dormiens Nunquam Titillandus</p>")
print("        <div class=\"quote-box\">")
print("            <p class=\"daily-quote\">\"" + quote["text"] + "\"</p>")
print("            <p class=\"quote-author\">- " + quote["author"] + "</p>")
print("        </div>")
print("    </div>")
print("")
print("    <div class=\"content\">")
print("        <h2>Welcome to Hogwarts</h2>")
print("        <p>Founded over a thousand years ago by Godric Gryffindor, Helga Hufflepuff, Rowena Ravenclaw, and Salazar Slytherin, Hogwarts is one of the finest magical institutions in the wizarding world.</p>")
print("")
print("        <h3>The Four Houses</h3>")
print("        <ul>")
print("            <li><a href=\"/gryffindor.html\">Gryffindor</a> - Where dwell the brave at heart</li>")
print("            <li><a href=\"/hufflepuff.html\">Hufflepuff</a> - Where they are just and loyal</li>")
print("            <li><a href=\"/ravenclaw.html\">Ravenclaw</a> - Where those of wit and learning will find their kind</li>")
print("            <li><a href=\"/slytherin.html\">Slytherin</a> - Where you'll make your real friends</li>")
print("        </ul>")
print("")
print("        <h3>Important Information</h3>")
print("        <ul>")
print("            <li><a href=\"/schedule.pl\">Class Schedule</a></li>")
print("            <li><a href=\"/rules.pl\">School Rules</a></li>")
print("        </ul>")
print("    </div>")
print("</body>")
print("</html>")
