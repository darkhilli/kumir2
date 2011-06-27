#ifndef RUN_INTERFACE
#define RUN_INTERFACE

#include <QtCore>

namespace Shared {

enum ProgramFormat {
    FormatText,
    FormatBinary
};

class RunInterface {
public:
    virtual bool loadProgram(QIODevice * source, ProgramFormat format) = 0;
    virtual void runContinuous() = 0;
    virtual void runStepOver() = 0;
    virtual void runStepInto() = 0;
    virtual void runStepOut() = 0;
    virtual void terminate() = 0;
    virtual bool hasMoreInstructions() const = 0;
    virtual QString error() const = 0;

    virtual void replyToRequest(const QVariantList & message) = 0;
/*
 * NOTE: signals should not be declared as virtuals in interface, but
 *       must be implemented in derivative classes
 *
signals:
    void stopped();
    void externalRequest(const QString &actorPluginName, const QString & algName, const QVariantList & args);
    void inputRequest(const QString & format);
    void outputRequest(const QVariantList & output);
*/
};

}

Q_DECLARE_INTERFACE(Shared::RunInterface, "kumir2.run")

#endif
