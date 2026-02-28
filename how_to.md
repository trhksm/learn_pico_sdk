# setup
ダウンロード(Appleシリコン用)
brew tap ArmMbed/homebrew-formulae
arm-gnu-toolchain-15.2.rel1-darwin-arm64-arm-none-eabi.pkg

VScode
File->Add folder to Workspace
[(pico W用設定)(一回め)
cmd+shift+p Open Wordspace Settings (JSON)
"cmake.configureSettings": {
  "PICO_BOARD": "pico_w"
}を追加
CMake: Delete Cache and Reconfigure
CMake: Build
]
cmd+shift+p CMake: Select Active Folder
cmd+shift+p CMake: Configure
cmd+shift+p CMake: Build

BOOTSELを押しながらUSBを差し込む
build/*.uf2をラズピコフォルダに入れる




# 開発方法
template(
    CMakeLists.txt
    main.c
    pico_sdk_import.cmake
)をコピーする
CMakeLists.txt(
    projectnameの変更
    ライブラリの追加
    printfの出力先
)
main.c