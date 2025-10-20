#!/usr/bin/env python3

import time

time.sleep(20)
print("Content-Type: text/html; charset=utf-8\r\n")
print("<html><body><h1>This should timeout</h1></body></html>")
