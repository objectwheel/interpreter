ios {
    installProject.files = $$files($$PWD/../../project/*)
    installProject.path =
    QMAKE_BUNDLE_DATA += installProject
}
