macx {
    installProject.files = $$files($$PWD/../../project/*)
    installProject.path = Contents/MacOS/owprj
    QMAKE_BUNDLE_DATA += installProject
}
