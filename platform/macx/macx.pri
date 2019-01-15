macx {
    installProject.files = $$files($$PWD/../../project/*)
    installProject.path = Contents/MacOS
    QMAKE_BUNDLE_DATA += installProject
}
