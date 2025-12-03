import os
from PIL import Image, ExifTags
import argparse


parser = argparse.ArgumentParser(description="gps metadatas")
parser.add_argument("dossier",nargs="?", default="images")
args = parser.parse_args()

dossier = args.dossier


def get_gps(exif_data):
    gps_info = {}
    for key, val in exif_data.items():
        tag = ExifTags.TAGS.get(key)
        if tag == "GPSInfo":
            for t in val:
                sub_tag = ExifTags.GPSTAGS.get(t, t)
                gps_info[sub_tag] = val[t]
    return gps_info

for fichier in os.listdir(dossier):
    chemin = os.path.join(dossier, fichier)
    if not fichier.lower().endswith((".jpg", ".jpeg", ".tiff")):
        continue
    try:
        img = Image.open(chemin)
        exif_data = img._getexif()
        if exif_data:
            gps = get_gps(exif_data)
            if gps:
                print(f"{fichier} :", gps)
            else:
                print(f"{fichier} : pas de GPS")
        else:
            print(f"{fichier} : pas de GPS")
    except Exception as e:
        print(f"{fichier} : erreur {e}")
