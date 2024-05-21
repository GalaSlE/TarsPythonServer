FROM docker:19.03 AS idocker
RUN mv $(command -v  docker) /tmp/docker

FROM tarscloud/tars-node:latest

# Install
RUN apt update 
RUN apt install -y python3-dev python3-pip exiftool && apt-get clean

# python3 environment
COPY requirements.txt ./
RUN pip3 install -vvv --no-cache-dir -r requirements.txt
COPY PythonScript /root/PythonScript

ENTRYPOINT [ "/sbin/entrypoint.sh" ]
