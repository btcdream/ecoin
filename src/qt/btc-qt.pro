OTHER_FILES += \
    macdockiconhandler.mm \
    btc-qt.pro.user
INCLUDEPATH += D:\boost_1_55_0\boost_1_55_0
INCLUDEPATH += D:\db-4.7.25.NC\build_windows
INCLUDEPATH += D:\openssl-0.9.8h\outinc
INCLUDEPATH += D:\openssl-0.9.8h
INCLUDEPATH += D:\bitcoin-0.8.5-win32\src\src


CONFIG += exceptions
HEADERS += \
    walletview.h \
    walletstack.h \
    walletmodel.h \
    walletframe.h \
    transactionview.h \
    transactiontablemodel.h \
    transactionrecord.h \
    transactionfilterproxy.h \
    transactiondescdialog.h \
    transactiondesc.h \
    splashscreen.h \
    signverifymessagedialog.h \
    sendcoinsentry.h \
    sendcoinsdialog.h \
    rpcconsole.h \
    qvaluecombobox.h \
    qvalidatedlineedit.h \
    qrcodedialog.h \
    paymentserver.h \
    overviewpage.h \
    optionsmodel.h \
    optionsdialog.h \
    notificator.h \
    monitoreddatamapper.h \
    macdockiconhandler.h \
    guiutil.h \
    guiconstants.h \
    editaddressdialog.h \
    csvmodelwriter.h \
    clientmodel.h \
    bitcoinunits.h \
    bitcoingui.h \
    bitcoinamountfield.h \
    bitcoinaddressvalidator.h \
    askpassphrasedialog.h \
    addresstablemodel.h \
    addressbookpage.h \
    aboutdialog.h

SOURCES += \
    walletview.cpp \
    walletstack.cpp \
    walletmodel.cpp \
    walletframe.cpp \
    transactionview.cpp \
    transactiontablemodel.cpp \
    transactionrecord.cpp \
    transactionfilterproxy.cpp \
    transactiondescdialog.cpp \
    transactiondesc.cpp \
    splashscreen.cpp \
    signverifymessagedialog.cpp \
    sendcoinsentry.cpp \
    sendcoinsdialog.cpp \
    rpcconsole.cpp \
    qvaluecombobox.cpp \
    qvalidatedlineedit.cpp \
    qrcodedialog.cpp \
    paymentserver.cpp \
    overviewpage.cpp \
    optionsmodel.cpp \
    optionsdialog.cpp \
    notificator.cpp \
    monitoreddatamapper.cpp \
    guiutil.cpp \
    editaddressdialog.cpp \
    csvmodelwriter.cpp \
    clientmodel.cpp \
    bitcoinunits.cpp \
    bitcoinstrings.cpp \
    bitcoingui.cpp \
    bitcoinamountfield.cpp \
    bitcoinaddressvalidator.cpp \
    bitcoin.cpp \
    askpassphrasedialog.cpp \
    addresstablemodel.cpp \
    addressbookpage.cpp \
    aboutdialog.cpp

RESOURCES += \
    bitcoin.qrc
QT       += core gui
QT += widgets
QT += network
