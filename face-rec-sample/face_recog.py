####################################################################################################
# @file name	: face_recog.py
# @Author	: Tanmay Kothale, Varun Mehta, Amey Dashaputre
# @Date	: 04/26/2022
# @References	: 1. https://github.com/ageitgey/face_recognition
#		  2. https://github.com/cu-ecen-aeld/final-project-saloni1307/blob/master/base_external/rootfs_overlay/etc/project/face_recog.py
# @Brief	: Application code that uses facial recognition algorithm to monitor attendance
####################################################################################################

##########################Importing libraries required for the program #############################
import face_recognition
import cv2
import numpy as np
from PIL import Image
#import serial
import os,time

#sim800l = serial.Serial('/dev/ttyAMA0', baudrate = 9600,timeout=1)

#previous ="Unknown"
#count=0

###########################code for face recognition started##############################

file = 'image.jpg'

# load the picture whose face has to be recognised.
tanmay_image = face_recognition.load_image_file("/etc/face-rec-sample/photos/tanmay.jpg")
tanmay_face_encoding = face_recognition.face_encodings(tanmay_image)[0]

varun_image = face_recognition.load_image_file("/etc/face-rec-sample/photos/varun.jpg")
varun_face_encoding = face_recognition.face_encodings(varun_image)[0]

amey_image = face_recognition.load_image_file("/etc/face-rec-sample/photos/amey.jpg")
amey_face_encoding = face_recognition.face_encodings(amey_image)[0]

# start your webcam
video_capture = cv2.VideoCapture(0)

# Create arrays of known face encodings and their names
known_face_encodings = [
    tanmay_face_encoding,
    varun_face_encoding,
    amey_face_encoding
    ]
    
known_face_names = [
    "Tanmay Kothale",
    "Varun Mehta",
    "Amey Dashaputre"
    ]

# Initialize some variables
face_locations = []
face_encodings = []
face_names = []
process_this_frame = True

# Create flags for each known person to mark attendance
tanmay_flag = False
amey_flag = False
varun_flag = False

################infinite loop to recognise face in the frame of the camera############
while True:
        
    # Grab a single frame of video
    ret, frame = video_capture.read()

    # Resize frame of video to 1/4 size for faster face recognition processing
    small_frame = cv2.resize(frame, (0, 0), fx=0.25, fy=0.25)

    #Convert the image from BGR color (which OpenCV uses) to RGB color (which face_recognition uses)
    rgb_small_frame = small_frame[:, :, ::-1]

    # Only process every other frame of video to save time
    if process_this_frame:
        # Find all the faces and face encodings in the current frame of video
        face_locations = face_recognition.face_locations(rgb_small_frame)
        face_encodings = face_recognition.face_encodings(rgb_small_frame, face_locations)
        
        face_names = []
        for face_encoding in face_encodings:
            # See if the face is a match for the known face(s)
            matches = face_recognition.compare_faces(known_face_encodings, face_encoding)
            name = "Unknown"

            face_distances = face_recognition.face_distance(known_face_encodings, face_encoding)
            best_match_index = np.argmin(face_distances)
            
            #if face encodings are matched with a known face, determine the face and store their name to a file
            if matches[best_match_index]:
                name = known_face_names[best_match_index]
                
                face_names.append(name)
         #---------------------------------------------------------#  
                if not tanmay_flag:	#tanmay is not present yet
                    if best_match_index == 0:
                        with open('names.txt','a') as f:
                            f.write(name)
                            f.write('\n')
                            f.close()
                        tanmay_flag = True	#attendance for tanmay has been marked
                else:
                    print("Tanmay Kothale is Already present!")	#tanmay is already present
         #---------------------------------------------------------#               
                if not varun_flag:	#varun is not present yet
                    if best_match_index == 1:
                        with open('names.txt','a') as f:
                            f.write(name)
                            f.write('\n')
                            f.close()
                        varun_flag = True 	#attendance for varun has been marked
                else:
                    print("Varun Mehta is Already present!") #varun is already present
          #---------------------------------------------------------#          
                if not amey_flag:	#amey is not present yet
                    if best_match_index == 2:
                        with open('names.txt','a') as f:
                            f.write(name)
                            f.write('\n')
                            f.close()
                        amey_flag = True	#attendance for amey has been marked
                else:
                    print("Amey Dashaputre is Already present!") #amey is already present
          #---------------------------------------------------------#  
                name = ''  #clear the string that stored the name for next iteration


    process_this_frame = not process_this_frame	#to process a new frame
    
video_capture.release()
cv2.destroyAllWindows()


