import argparse
import os
from PIL import Image, ExifTags
import piexif
import shutil

parser = argparse.ArgumentParser(description="crawler img")
parser.add_argument("dossier_a_trier", nargs="?", default="challenge2/images_a_trier")
parser.add_argument("dossier_gps", nargs="?", default="challenge2/images_gps")
parser.add_argument("dossier_gps_modifiees", nargs="?", default="challenge2/images_gps_modifiees")
parser.add_argument(
    "localisation",
    nargs="?",
    default="48.8219,2.3582",  # Maison Blanche
    help="Coordonnées GPS au format lat,lon"
)
args = parser.parse_args()

dossier_a_trier = args.dossier_a_trier
dossier_gps = args.dossier_gps
dossier_gps_modifiees = args.dossier_gps_modifiees

os.makedirs(dossier_a_trier, exist_ok=True)
os.makedirs(dossier_gps, exist_ok=True)
os.makedirs(dossier_gps_modifiees, exist_ok=True)

lat_str, lon_str = args.localisation.split(",")

latitude = float(lat_str)
longitude = float(lon_str)

GPS_LAT = decimal_to_dms(latitude)
GPS_LON = decimal_to_dms(longitude)

GPS_LAT_REF = "N" if latitude >= 0 else "S"
GPS_LON_REF = "E" if longitude >= 0 else "W"

def decimal_to_dms(decimal):
    decimal = float(decimal)
    deg = int(decimal)
    min_float = abs(decimal - deg) * 60
    minute = int(min_float)
    sec = int((min_float - minute) * 60)
    return ((abs(deg), 1), (minute, 1), (sec, 1))

def get_gps(exif_data):
    gps_info = {}
    for key, val in exif_data.items():
        tag = ExifTags.TAGS.get(key)
        if tag == "GPSInfo":
            for t in val:
                sub_tag = ExifTags.GPSTAGS.get(t, t)
                gps_info[sub_tag] = val[t]
    return gps_info

def modifier_gps(chemin_fichier):
    exif_dict = piexif.load(chemin_fichier)

    exif_dict["GPS"] = {
        piexif.GPSIFD.GPSLatitudeRef: GPS_LAT_REF,
        piexif.GPSIFD.GPSLatitude: GPS_LAT,
        piexif.GPSIFD.GPSLongitudeRef: GPS_LON_REF,
        piexif.GPSIFD.GPSLongitude: GPS_LON
    }

    exif_bytes = piexif.dump(exif_dict)

    nom_fichier = os.path.basename(chemin_fichier)
    destination = os.path.join(dossier_gps_modifiees, nom_fichier)

    piexif.insert(exif_bytes, chemin_fichier)
    shutil.move(chemin_fichier, destination)

for fichier in os.listdir(dossier_a_trier):
    chemin = os.path.join(dossier_a_trier, fichier)

    if not fichier.lower().endswith((".jpg", ".jpeg", ".tiff")):
        continue

    try:
        img = Image.open(chemin)
        exif_data = img._getexif()

        if exif_data:
            gps = get_gps(exif_data)

            if gps:
                shutil.move(chemin, os.path.join(dossier_gps, fichier))

            else:
                modifier_gps(chemin)

        else:
            modifier_gps(chemin)

    except Exception as e:
        print(f"{fichier} : erreur {e}")
