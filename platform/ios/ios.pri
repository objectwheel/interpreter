ios {
    installProject.files = $$files($$PWD/../../project/*)
    installProject.path = owprj
    QMAKE_BUNDLE_DATA += installProject
}
