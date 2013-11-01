PR2_Spatula_Vision
==================
Use cv_hsv_test.py to threshold hsv values for a given image. 
Run python cv_hsv_test.py image_name

After the threshold, put the ideal hsv threadhold values in the src/listener.cpp file. 
Run rosmake vision, and then run bin/.pcl_listener

This finds the centroid of the target object.
