#include "cuiseparator.hpp"
 
namespace EE { namespace UI {

cUISeparator::cUISeparator( cUIControlAnim::CreateParams Params ) :
	cUIControlAnim( Params )
{
	ApplyDefaultTheme();
}

cUISeparator::~cUISeparator() {
}

Uint32 cUISeparator::Type() const {
	return UI_TYPE_SEPARATOR;
}

bool cUISeparator::IsType( const Uint32& type ) const {
	return cUISeparator::Type() == type ? true : cUIControlAnim::IsType( type );
}

void cUISeparator::SetTheme( cUITheme * Theme ) {
	cUIControl::SetTheme( Theme, "separator" );
	
	if ( NULL != mSkinState && NULL != mSkinState->GetSkin() && NULL != mSkinState->GetSkin()->GetShape( cUISkinState::StateNormal ) ) {
		Size( mSize.Width(), mSkinState->GetSkin()->GetShape( cUISkinState::StateNormal )->RealSize().Height() );
	}
}

}}
