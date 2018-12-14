#include "base58.h"

#include "hash.h"
#include "uint256.h"

#include <assert.h>
#include <stdint.h>
#include <string.h>
#include <vector>
#include <string>
#include <boost/variant/apply_visitor.hpp>
#include <boost/variant/static_visitor.hpp>
#include "pubkey.h"
#include "wallet.h"


//SignCompact() 通用签名
void on_signMessageButton_SM_clicked()
{ 
    /* Clear old signature to ensure users don't get confused on error with an old signature displayed */
    //ui->signatureOut_SM->clear();
    std::string addring;
    
    CTxDestination destination = CBitcoinAddress(addring).Get(); // DecodeDestination(addring);//ui->addressIn_SM->text().toStdString());
    if (!CBitcoinAddress(addring).IsValid()) {
        //ui->statusLabel_SM->setStyleSheet("QLabel { color: red; }");
        //ui->statusLabel_SM->setText(tr("The entered address is invalid.") + QString(" ") + tr("Please check the address and try again."));
        return;
    }
    const CKeyID* keyID = boost::get<CKeyID>(&destination);
    if (!keyID) {
        //ui->addressIn_SM->setValid(false);
        //ui->statusLabel_SM->setStyleSheet("QLabel { color: red; }");
        //ui->statusLabel_SM->setText(tr("The entered address does not refer to a key.") + QString(" ") + tr("Please check the address and try again."));
        return;
    }

    //WalletModel::UnlockContext ctx(model->requestUnlock());
    //if (!ctx.isValid())
    {
        //ui->statusLabel_SM->setStyleSheet("QLabel { color: red; }");
        //ui->statusLabel_SM->setText(tr("Wallet unlock was cancelled."));
     //   return;
    }

    CWallet *wallet;
    CKey key; // model  ==CCryptoKeyStore::GetKey(const CKeyID &address, CKey &keyOut) 
    if(! wallet->GetKey(*keyID, key))
    {
        //ui->statusLabel_SM->setStyleSheet("QLabel { color: red; }");
        //ui->statusLabel_SM->setText(tr("Private key for the entered address is not available."));
        return;
    }

    CHashWriter ss(SER_GETHASH, 0);
    ss << strMessageMagic;
    ss << std::string("testststt");//ui->messageIn_SM->document()->toPlainText().toStdString();

    std::vector<unsigned char> vchSig;
    if (!key.SignCompact(ss.GetHash(), vchSig))
    {
        //ui->statusLabel_SM->setStyleSheet("QLabel { color: red; }");
        //ui->statusLabel_SM->setText(QString("<nobr>") + tr("Message signing failed.") + QString("</nobr>"));
        return;
    }

    //ui->statusLabel_SM->setStyleSheet("QLabel { color: green; }");
    //ui->statusLabel_SM->setText(QString("<nobr>") + tr("Message signed.") + QString("</nobr>"));

    //ui->signatureOut_SM->setText(QString::fromStdString(EncodeBase64(vchSig.data(), vchSig.size())));
}

//校验签名
void on_verifyMessageButton_VM_clicked()
{
    std::string addring;

    CTxDestination destination = CBitcoinAddress(addring).Get();//ui->addressIn_VM->text().toStdString());
    if (!CBitcoinAddress(addring).IsValid()) {
        //ui->statusLabel_VM->setStyleSheet("QLabel { color: red; }");
        //ui->statusLabel_VM->setText(tr("The entered address is invalid.") + QString(" ") + tr("Please check the address and try again."));
        return;
    }
    if (!boost::get<CKeyID>(&destination)) {
        //ui->addressIn_VM->setValid(false);
        //ui->statusLabel_VM->setStyleSheet("QLabel { color: red; }");
        //ui->statusLabel_VM->setText(tr("The entered address does not refer to a key.") + QString(" ") + tr("Please check the address and try again."));
        return;
    }

    bool fInvalid = false;
    std::vector<unsigned char> vchSig = DecodeBase64("sfsfsfsfsfdfs");//ui->signatureIn_VM->text().toStdString().c_str(), &fInvalid);

    if (fInvalid)
    {
        //ui->signatureIn_VM->setValid(false);
        //ui->statusLabel_VM->setStyleSheet("QLabel { color: red; }");
        //ui->statusLabel_VM->setText(tr("The signature could not be decoded.") + QString(" ") + tr("Please check the signature and try again."));
        return;
    }

    CHashWriter ss(SER_GETHASH, 0);
    ss << strMessageMagic;
    ss << std::string("asfasfsfsf") ;//ui->messageIn_VM->document()->toPlainText().toStdString();

    CPubKey pubkey;
    if (!pubkey.RecoverCompact(ss.GetHash(), vchSig))
    {
        //ui->signatureIn_VM->setValid(false);
        //ui->statusLabel_VM->setStyleSheet("QLabel { color: red; }");
        //ui->statusLabel_VM->setText(tr("The signature did not match the message digest.") + QString(" ") + tr("Please check the signature and try again."));
        return;
    }

    if (!(CTxDestination(pubkey.GetID()) == destination)) {
        //ui->statusLabel_VM->setStyleSheet("QLabel { color: red; }");		
		//ui->statusLabel_VM->setText(QString("<nobr>") + tr("Message verified.") + QString("</nobr>"));
	}
          
    //ui->statusLabel_VM->setStyleSheet("QLabel { color: green; }");
    //ui->statusLabel_VM->setText(QString("<nobr>") + tr("Message verified.") + QString("</nobr>"));
}

