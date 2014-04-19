#ifndef SQLEXPORT_H
#define SQLEXPORT_H

#include "plugins/genericexportplugin.h"
#include "sqlexport_global.h"
#include "config_builder.h"

CFG_CATEGORIES(SqlExportConfig,
     CFG_CATEGORY(SqlExport,
         CFG_ENTRY(QString, QueryTable,             QString::null)
         CFG_ENTRY(bool,    GenerateCreateTable,    false)
         CFG_ENTRY(bool,    IncludeQueryInComments, true)
     )
)

#define SQL_EXPORT_CFG CFG_INSTANCE(SqlExportConfig)

class SQLEXPORTSHARED_EXPORT SqlExport : public GenericExportPlugin
{
        Q_OBJECT

        SQLITESTUDIO_PLUGIN
        SQLITESTUDIO_PLUGIN_TITLE("SQL exporting format")
        SQLITESTUDIO_PLUGIN_DESC("Provides SQL format for exporting")
        SQLITESTUDIO_PLUGIN_VERSION(10000)
        SQLITESTUDIO_PLUGIN_AUTHOR("sqlitestudio.pl")

    public:
        SqlExport();

        QString getFormatName() const;
        ExportManager::StandardConfigFlags standardOptionsToEnable() const;
        CfgMain* getConfig() const;
        QString defaultFileExtension() const;
        QString getConfigFormName() const;
        bool beforeExportQueryResults(const QString& query, QList<QueryExecutor::ResultColumnPtr>& columns);
        bool exportQueryResultsRow(SqlResultsRowPtr row);
        bool afterExportQueryResults();
        bool beforeExportTable(const QString& database, const QString& table, const QStringList& columnNames, const QString& ddl, bool databaseExport);
        bool exportTableRow(SqlResultsRowPtr data);
        bool afterExportTable();
        bool beforeExportDatabase();
        bool exportIndex(const QString& database, const QString& name, const QString& ddl);
        bool exportTrigger(const QString& database, const QString& name, const QString& ddl);
        bool exportView(const QString& database, const QString& name, const QString& ddl);
        bool afterExportDatabase();

    private:
        void writeHeader();
        void writeBegin();
        void writeCommit();
        void writeFkDisable();
        QString getNameForObject(const QString& database, const QString& name, bool wrapped, Dialect dialect = Dialect::Sqlite3);
        QStringList rowToArgList(SqlResultsRowPtr row);

        QString theTable;
        QString columns;
        bool dbExport = false;

    public slots:
        void validateOptions();
};

#endif // SQLEXPORT_H