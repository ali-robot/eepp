#ifndef ECODE_LINTERPLUGIN_HPP
#define ECODE_LINTERPLUGIN_HPP

#include "../pluginmanager.hpp"
#include <eepp/config.hpp>
#include <eepp/system/mutex.hpp>
#include <eepp/system/threadpool.hpp>
#include <eepp/ui/uicodeeditor.hpp>
#include <memory>
#include <set>
using namespace EE;
using namespace EE::System;
using namespace EE::UI;

namespace ecode {

enum class LinterType { Notice, Warning, Error };

enum class MatchOrigin { Linter, Diagnostics };

struct Linter {
	std::vector<std::string> files;
	std::vector<std::string> warningPattern;
	bool columnsStartAtZero{ false };
	bool deduplicate{ false };
	bool useTmpFolder{ false };
	bool hasNoErrorsExitCode{ false };
	struct {
		int line{ 1 };
		int col{ 2 };
		int message{ 3 };
		int type{ -1 };
	} warningPatternOrder;
	std::string command;
	std::vector<Int64> expectedExitCodes{};
	int noErrorsExitCode{ 0 };
};

struct LinterMatch {
	std::string text;
	TextRange range;
	LinterType type{ LinterType::Error };
	String::HashType lineCache;
	MatchOrigin origin{ MatchOrigin::Linter };
	std::map<UICodeEditor*, Rectf> box;
};

class LinterPlugin : public UICodeEditorPlugin {
  public:
	static PluginDefinition Definition() {
		return { "linter",
				 "Linter",
				 "Use static code analysis tool used to flag programming errors, bugs, "
				 "stylistic errors, and suspicious constructs.",
				 LinterPlugin::New,
				 { 0, 1, 0 } };
	}
	static UICodeEditorPlugin* New( PluginManager* pluginManager );

	virtual ~LinterPlugin();

	std::string getId() { return Definition().id; }

	std::string getTitle() { return Definition().name; }

	std::string getDescription() { return Definition().description; }

	bool isReady() const { return mReady; }

	bool hasFileConfig();

	std::string getFileConfigPath();

	void onRegister( UICodeEditor* );

	void onUnregister( UICodeEditor* );

	void drawAfterLineText( UICodeEditor* editor, const Int64& index, Vector2f position,
							const Float& fontSize, const Float& lineHeight );

	void minimapDrawBeforeLineText( UICodeEditor*, const Int64&, const Vector2f&, const Vector2f&,
									const Float&, const Float& );

	void update( UICodeEditor* );

	bool onMouseMove( UICodeEditor*, const Vector2i&, const Uint32& flags );

	bool onMouseLeave( UICodeEditor*, const Vector2i&, const Uint32& );

	const Time& getDelayTime() const;

	void setDelayTime( const Time& delayTime );

	bool getEnableLSPDiagnostics() const;

	void setEnableLSPDiagnostics( bool enableLSPDiagnostics );

  protected:
	PluginManager* mManager{ nullptr };
	std::shared_ptr<ThreadPool> mPool;
	std::vector<Linter> mLinters;
	std::unordered_map<UICodeEditor*, std::vector<Uint32>> mEditors;
	std::set<TextDocument*> mDocs;
	std::unordered_map<UICodeEditor*, TextDocument*> mEditorDocs;
	std::unordered_map<TextDocument*, std::unique_ptr<Clock>> mDirtyDoc;
	std::unordered_map<TextDocument*, std::map<Int64, std::vector<LinterMatch>>> mMatches;
	Time mDelayTime{ Seconds( 0.5f ) };
	Mutex mDocMutex;
	Mutex mMatchesMutex;
	std::mutex mWorkMutex;
	std::condition_variable mWorkerCondition;
	Int32 mWorkersCount{ 0 };
	std::string mConfigPath;

	bool mReady{ false };
	bool mShuttingDown{ false };
	bool mHoveringMatch{ false };
	bool mEnableLSPDiagnostics{ true };
	std::set<std::string> mLanguagesDisabled;
	std::set<std::string> mLSPLanguagesDisabled;

	LinterPlugin( PluginManager* pluginManager );

	void load( PluginManager* pluginManager );

	void lintDoc( std::shared_ptr<TextDocument> doc );

	void runLinter( std::shared_ptr<TextDocument> doc, const Linter& linter,
					const std::string& path );

	Linter supportsLinter( std::shared_ptr<TextDocument> doc );

	void setDocDirty( TextDocument* doc );

	void setDocDirty( UICodeEditor* editor );

	void invalidateEditors( TextDocument* doc );

	std::string getMatchString( const LinterType& type );

	void loadLinterConfig( const std::string& path );

	size_t linterFilePatternPosition( const std::vector<std::string>& patterns );

	PluginRequestHandle processMessage( const PluginMessage& notification );

	TextDocument* getDocumentFromURI( const URI& uri );

	void eraseMatchesFromOrigin( TextDocument* doc, const MatchOrigin& origin );

	void insertMatches( TextDocument* doc, std::map<Int64, std::vector<LinterMatch>>& matches );

	void setMatches( TextDocument* doc, const MatchOrigin& origin,
					 std::map<Int64, std::vector<LinterMatch>>& matches );

	void tryHideHoveringMatch( UICodeEditor* editor );
};

} // namespace ecode

#endif // ECODE_LINTERPLUGIN_HPP
