/*#include "libraries.h"

QTextStream *out =0;

void Log::creerLog(void)
{
   QFile* fichierLog = new QFile("monFile");
   if(fichierLog->open(QIODevice::WriteOnly))
   {
      out = new QTextStream(fichierLog);
      qInstallMsgHandler(Log::informationLog);
   }
   else
   {
      delete fichierLog;
   }
}

void Log::informationLog(QtMsgType type, const char *message)
{
    QString date = QDateTime::currentDateTime().toString(QLatin1String("[dd.MM.yy hh.mm.ss] " ));
    (*out) << date << message << endl;
}

void Log::information(char* message)
{
    qDebug() << message;
}
*/
