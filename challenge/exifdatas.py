import os
from PIL import Image, ExifTags
import argparse


parser = argparse.ArgumentParser(description="exif datas")
parser.add_argument("dossier",nargs="?", default="images")
args = parser.parse_args()

dossier = args.dossier

fichiers = os.listdir(dossier)
fichiers.sort()

for fichier in fichiers:
    chemin = os.path.join(dossier, fichier)

    if not fichier.lower().endswith((".jpg", ".jpeg", ".png", ".webp", ".tiff")):
        continue

    print("\n" + "=" * 50)
    print(f"Fichier : {fichier}")

    try:
        img = Image.open(chemin)

        print(f"Format     : {img.format}")
        print(f"Taille     : {img.size[0]} x {img.size[1]}")
        print(f"Mode       : {img.mode}")

        exif_data = img._getexif()

        if exif_data:
            print("\nEXIF :")
            for tag_id, value in exif_data.items():
                tag = ExifTags.TAGS.get(tag_id, tag_id)
                print(f"  {tag} : {value}")
        else:
            print("\nAucun EXIF trouve")

    except Exception as e:
        print("Erreur :", e)
