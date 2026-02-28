# setup
ダウンロード(Appleシリコン用)<br>
brew tap ArmMbed/homebrew-formulae<br>
arm-gnu-toolchain-15.2.rel1-darwin-arm64-arm-none-eabi.pkg<br>

## VScode
File->Add folder to Workspace <br>
[(pico W用設定)(一回め) <br>
cmd+shift+p Open Wordspace Settings (JSON)<br>
"cmake.configureSettings": {<br>
  "PICO_BOARD": "pico_w"<br>
}を追加<br>
CMake: Delete Cache and Reconfigure<br>
CMake: Build<br>
]<br>
cmd+shift+p CMake: Select Active Folder<br>
cmd+shift+p CMake: Configure<br>
cmd+shift+p CMake: Build<br>

BOOTSELを押しながらUSBを差し込む<br>
build/*.uf2をラズピコフォルダに入れる<br>




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
