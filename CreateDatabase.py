#!/usr/bin/python

import sqlite3
conn = sqlite3.connect('../DB/midiQueries.db')

c = conn.cursor()

# Turn on foreign key support
c.execute("PRAGMA foreign_keys = ON")

# Create midi table
c.execute('''CREATE TABLE midi
	     (fileName TEXT NOT NULL,
	      artist TEXT NOT NULL,
	      song TEXT NOT NULL,
	      contourMap TEXT NOT NULL,
	      noteMap TEXT NOT NULL,
	      PRIMARY KEY(fileName))''')

# Save the changes
conn.commit()

# Close the connection
conn.close()
