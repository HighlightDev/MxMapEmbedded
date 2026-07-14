docker build -t mx-map-embedded:alpine-x86 .

docker run -it --mount type=bind,src=$(pwd),dst=/home/MxMap mx-map-embedded:alpine-x86

cd /home/MxMap/


# 1. Создаем именованный пайп с именем dbg_pipe
mkfifo /tmp/dbg_pipe

# 2. Записываем строку в пайп (в фоновом режиме '&', 
# так как запись заблокирует скрипт, пока кто-то не начнет читать)
echo "Hello!" > /tmp/dbg_pipe &

# 3. Читаем строку из пайпа
read response < /tmp/dbg_pipe
echo "Получено из пайпа: $response"

# 4. Удаляем пайп после использования
rm tmp/dbg_pipe

