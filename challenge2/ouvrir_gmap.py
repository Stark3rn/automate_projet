import os
import argparse
import webbrowser
from PIL import Image, ExifTags

parser = argparse.ArgumentParser(description="Ouvre Google Maps avec le GPS de chaque image")
parser.add_argument("dossier_gps", nargs="?", default="challenge2/images_gps")
args = parser.parse_args()

dossier_gps = args.dossier_gps


def get_gps(exif_data):
    gps_info = {}
    for key, val in exif_data.items():
        tag = ExifTags.TAGS.get(key)
        if tag == "GPSInfo":
            for t in val:
                sub_tag = ExifTags.GPSTAGS.get(t, t)
                gps_info[sub_tag] = val[t]
    return gps_info

def dms_to_decimal(dms, ref):
    deg = float(dms[0])
    minute = float(dms[1])
    sec = float(dms[2])

    decimal = deg + (minute / 60) + (sec / 3600)

    if ref in ["S", "W"]:
        decimal = -decimal

    return decimal


for fichier in os.listdir(dossier_gps):
    chemin = os.path.join(dossier_gps, fichier)

    if not fichier.lower().endswith((".jpg", ".jpeg", ".tiff")):
        continue

    try:
        img = Image.open(chemin)
        exif_data = img._getexif()

        if not exif_data:
            print(f"{fichier} : pas de EXIF")
            continue

        gps = get_gps(exif_data)

        if not gps:
            print(f"{fichier} : pas de GPS")
            continue

        lat = dms_to_decimal(gps["GPSLatitude"], gps["GPSLatitudeRef"])
        lon = dms_to_decimal(gps["GPSLongitude"], gps["GPSLongitudeRef"])

        print(f"{fichier} -> {lat}, {lon}")

        url = f"https://www.google.com/maps?q={lat},{lon}"
        webbrowser.open(url)

    except Exception as e:
        print(f"{fichier} : erreur {e}")
