# Docker setup
docker build -t mx-map-embedded:alpine-x86 .

docker run -it --mount type=bind,src=$(pwd),dst=/home/MxMap mx-map-embedded:alpine-x86

cd /home/MxMap/

# Simulate device
## 1. Create named pipe dbg_pipe
mkfifo /tmp/dbg_pipe

## 2. Write into pipe (in background mode '&' to prevent block of terminal untill someone starts to read from the pipe)
echo "Hello!" > /tmp/dbg_pipe &

## 3. Start MxMaxEmbedded
./MxMaxEmbedded -o "output_directory" -g /tmp/dbg_pipe

### 4. Delete pipe after the usage
rm /tmp/dbg_pipe

# Service setup
chmod +x /home/MxMap/deploy/mxmapembedded
cp /home/MxMap/deploy/mxmapembedded /etc/init.d/
rc-update add mxmapembedded default
rc-service mxmapembedded start
