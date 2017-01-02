echo "Copying files to Google Drive folder..."
cp -urf ./* ~/Google\ Drive/Grado\ Videojuego\ -\ CITM/Curso3_Semestre1/Motores/ 
echo "Files copied! Starting Google Drive..."
"C:\Program Files (x86)\Google\Drive\googledrivesync.exe" & 
