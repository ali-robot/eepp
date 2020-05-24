#include <eepp/ui/doc/syntaxhighlighter.hpp>
#include <eepp/ui/doc/syntaxtokenizer.hpp>

namespace EE { namespace UI { namespace Doc {

SyntaxHighlighter::SyntaxHighlighter( TextDocument* doc ) : mDoc( doc ) {
	reset();
}

void SyntaxHighlighter::reset() {
	mLines.clear();
	mFirstInvalidLine = 0;
	mMaxWantedLine = 0;
}

void SyntaxHighlighter::invalidate( const size_t& line ) {
	mFirstInvalidLine = line;
	mMaxWantedLine = eemin( mMaxWantedLine, mDoc->linesCount() );
}

TokenizedLine SyntaxHighlighter::tokenizeLine( const size_t& line, const int& state ) {
	TokenizedLine tokenizedLine;
	tokenizedLine.initState = state;
	tokenizedLine.text = mDoc->line( line );
	auto res = SyntaxTokenizer::tokenize( mDoc->getSyntaxDefinition(), tokenizedLine.text, state );
	tokenizedLine.tokens = res.first;
	tokenizedLine.state = res.second;
	return tokenizedLine;
}

const std::vector<SyntaxToken>& SyntaxHighlighter::getLine( const size_t& index ) {
	auto it = mLines.find( index );
	if ( it == mLines.end() ||
		 ( index < mDoc->linesCount() && mDoc->line( index ) != it->second.text ) ) {
		int prevState = SYNTAX_TOKENIZER_STATE_NONE;
		if ( index > 0 ) {
			auto prevIt = mLines.find( index - 1 );
			if ( prevIt != mLines.end() ) {
				prevState = prevIt->second.state;
			}
		}
		mLines[index] = tokenizeLine( index, prevState );
		mMaxWantedLine = eemax( mMaxWantedLine, index );
		return mLines[index].tokens;
	}
	return it->second.tokens;
}

}}} // namespace EE::UI::Doc
