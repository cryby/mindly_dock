# Mindly Dock
Cross-platform app for MacOS and Windows, that turns your iPhone into a Dumb Phone, written in C++, leveraging libimobiledevice and iOS profiles

# Showcase
![app showcase](https://i.imgur.com/ZjUd2XM.png)
![app showcase #2](https://i.imgur.com/QiZbpjw.png)
![app showcase #3](https://i.imgur.com/PeMEtvM.jpeg)

## Build

### Dependencies (macOS)
```bash
brew install glfw libimobiledevice libplist
```

### Compile
```bash
mkdir build && cd build
cmake ..
make
```