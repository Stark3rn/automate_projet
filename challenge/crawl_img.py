from playwright.sync_api import sync_playwright
import requests
import os
from urllib.parse import urljoin, urlparse
from bs4 import BeautifulSoup
import argparse


parser = argparse.ArgumentParser(description="crawler img")
parser.add_argument("url", nargs="?", default="https://www.flickr.com/search/?license=9,10&text=car")
parser.add_argument("dossier",nargs="?", default="images")
args = parser.parse_args()

url = args.url
dossier = args.dossier

os.makedirs(dossier, exist_ok=True)

with sync_playwright() as p:
    browser = p.chromium.launch(headless=True)
    context = browser.new_context(viewport={"width":1280,"height":800})

    page = context.new_page()
    page.goto(url, wait_until="domcontentloaded", timeout=120000)
    page.wait_for_selector("a.overlay", timeout=15000)

    links = page.eval_on_selector_all("a.overlay", "els => els.map(a => a.href).slice(0,20)")

    browser.close()


for i, link in enumerate(links, start=1):
    try:
        r = requests.get(link, timeout=15)
        r.raise_for_status()

        soup = BeautifulSoup(r.text, "html.parser")
        img_tag = soup.find("img", {"class": "main-photo"})
        if not img_tag:
            print(f"Pas d'images pour {link}")
            continue

        img_url = img_tag.get("src") 

        if img_url.startswith("//"):
            img_url = "https:" + img_url

        if not img_url:
            continue

        nom = os.path.basename(urlparse(img_url).path)
        chemin = os.path.join(dossier, nom)

        img_data = requests.get(img_url, timeout=20).content
        with open(chemin, "wb") as f:
            f.write(img_data)

        print(f"Image {i} downloaded : {nom}")

    except Exception as e:
        print(f"Err : image {i} :", e)
