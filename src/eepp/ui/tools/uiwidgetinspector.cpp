#include <eepp/scene/scenemanager.hpp>
#include <eepp/ui/models/csspropertiesmodel.hpp>
#include <eepp/ui/models/widgettreemodel.hpp>
#include <eepp/ui/tools/uiwidgetinspector.hpp>
#include <eepp/ui/uicheckbox.hpp>
#include <eepp/ui/uiscenenode.hpp>
#include <eepp/ui/uitableview.hpp>
#include <eepp/ui/uitreeview.hpp>
#include <eepp/ui/uiwindow.hpp>
#include <eepp/window/input.hpp>
#include <eepp/window/window.hpp>

using namespace EE::Window;
using namespace EE::UI::Models;
using namespace EE::Scene;

namespace EE { namespace UI { namespace Tools {

UIWindow* UIWidgetInspector::create( UISceneNode* sceneNode, const Float& menuIconSize,
									 std::function<void()> highlightToggle,
									 std::function<void()> drawBoxesToggle,
									 std::function<void()> drawDebugDataToggle ) {
	if ( sceneNode->getRoot()->hasChild( "widget-tree-view" ) )
		return nullptr;
	UIWindow* uiWin = UIWindow::New();
	uiWin->setId( "widget-tree-view" );
	uiWin->setMinWindowSize( 600, 400 );
	uiWin->setWindowFlags( UI_WIN_DEFAULT_FLAGS | UI_WIN_RESIZEABLE | UI_WIN_MAXIMIZE_BUTTON );
	UIWidget* cont = sceneNode->loadLayoutFromString( R"xml(
	<vbox lw="mp" lh="mp">
		<hbox lw="wc" lh="wc">
			<pushbutton id="pick_widget" icon="icon(cursor-pointer, 16dp)" text='@string(pick_widget, "Pick Widget")' text-as-fallback="true" />
			<CheckBox id="debug-draw-highlight" text='@string(debug_draw_highlight, "Highlight Focus & Hover")' margin-left="4dp" lg="center" />
			<CheckBox id="debug-draw-boxes" text='@string(debug_draw_boxes, "Draw Boxes")' margin-left="4dp" lg="center" />
			<CheckBox id="debug-draw-debug-data" text='@string(debug_draw_debug_data, "Draw Debug Data")' margin-left="4dp" lg="center" />
		</hbox>
		<Splitter layout_width="match_parent" lh="fixed" lw8="1" splitter-partition="50%">
			<treeview lw="fixed" lh="mp" />
			<tableview lw="fixed" lh="mp" />
		</Splitter>
	</vbox>
	)xml",
													  uiWin->getContainer() );
	UITreeView* widgetTree = cont->findByType<UITreeView>( UI_TYPE_TREEVIEW );
	widgetTree->setHeadersVisible( true );
	widgetTree->setExpanderIconSize( menuIconSize );
	widgetTree->setAutoColumnsWidth( true );
	auto model = WidgetTreeModel::New( sceneNode );
	widgetTree->setModel( model );
	widgetTree->tryOpenModelIndex( model->getRoot() );
	UITableView* tableView = cont->findByType<UITableView>( UI_TYPE_TABLEVIEW );
	tableView->setAutoColumnsWidth( true );
	tableView->setHeadersVisible( true );
	widgetTree->setOnSelection( [&, tableView]( const ModelIndex& index ) {
		Node* node = static_cast<Node*>( index.internalData() );
		if ( node->isWidget() ) {
			tableView->setModel( node->isWidget()
									 ? CSSPropertiesModel::create( node->asType<UIWidget>() )
									 : CSSPropertiesModel::create() );
		}
	} );

	UIPushButton* button = cont->find<UIPushButton>( "pick_widget" );
	button->addEventListener(
		Event::MouseClick, [sceneNode, widgetTree, tableView]( const Event* event ) {
			if ( event->asMouseEvent()->getFlags() & EE_BUTTON_LMASK ) {
				bool wasHighlightOver = sceneNode->getHighlightOver();
				sceneNode->setHighlightOver( true );
				sceneNode->getEventDispatcher()->setDisableMousePress( true );
				sceneNode->runOnMainThread( [sceneNode, widgetTree, tableView, wasHighlightOver]() {
					checkWidgetPick( sceneNode, widgetTree, wasHighlightOver, tableView );
				} );
			}
		} );

	cont->find<UICheckBox>( "debug-draw-highlight" )
		->setChecked( sceneNode->getHighlightOver() )
		->addEventListener( Event::OnValueChange, [sceneNode, highlightToggle]( const auto* ) {
			if ( highlightToggle ) {
				highlightToggle();
			} else {
				sceneNode->setHighlightFocus( !sceneNode->getHighlightFocus() );
				sceneNode->setHighlightOver( !sceneNode->getHighlightOver() );
			}
		} );

	cont->find<UICheckBox>( "debug-draw-boxes" )
		->setChecked( sceneNode->getDrawBoxes() )
		->addEventListener( Event::OnValueChange, [sceneNode, drawBoxesToggle]( const auto* ) {
			if ( drawBoxesToggle ) {
				drawBoxesToggle();
			} else {
				sceneNode->setDrawBoxes( !sceneNode->getDrawBoxes() );
			}
		} );

	cont->find<UICheckBox>( "debug-draw-debug-data" )
		->setChecked( sceneNode->getDrawDebugData() )
		->addEventListener( Event::OnValueChange, [sceneNode, drawDebugDataToggle]( const auto* ) {
			if ( drawDebugDataToggle ) {
				drawDebugDataToggle();
			} else {
				sceneNode->setDrawDebugData( !sceneNode->getDrawDebugData() );
			}
		} );

	uiWin->center();

	Uint32 winCb = sceneNode->addEventListener( Event::OnWindowAdded, [&, sceneNode, uiWin](
																		  const Event* event ) {
		UIWindow* eWin = event->asWindowEvent()->getWindow()->asType<UIWindow>();
		if ( eWin != uiWin ) {
			Uint32 winRdCb =
				eWin->addEventListener( Event::OnWindowReady, [uiWin]( const Event* eWinEvent ) {
					uiWin->toFront();
					eWinEvent->getNode()->removeEventListener( eWinEvent->getCallbackId() );
				} );
			uiWin->addEventListener( Event::OnWindowClose, [sceneNode, winRdCb]( const Event* ) {
				if ( !SceneManager::instance()->isShootingDown() )
					sceneNode->removeEventListener( winRdCb );
			} );
		}
	} );
	uiWin->addEventListener( Event::OnWindowClose, [sceneNode, winCb]( const Event* ) {
		if ( !SceneManager::instance()->isShootingDown() )
			sceneNode->removeEventListener( winCb );
	} );
	return uiWin;
}

void UIWidgetInspector::checkWidgetPick( UISceneNode* sceneNode, UITreeView* widgetTree,
										 bool wasHighlightOver, UITableView* tableView ) {
	Input* input = sceneNode->getWindow()->getInput();
	if ( input->getClickTrigger() & EE_BUTTON_LMASK ) {
		Node* node = sceneNode->getEventDispatcher()->getMouseOverNode();
		WidgetTreeModel* model = static_cast<WidgetTreeModel*>( widgetTree->getModel() );
		ModelIndex index( model->getModelIndex( node ) );
		widgetTree->setSelection( index );
		sceneNode->setHighlightOver( wasHighlightOver );
		sceneNode->getEventDispatcher()->setDisableMousePress( false );
	} else {
		sceneNode->runOnMainThread( [sceneNode, widgetTree, wasHighlightOver, tableView]() {
			checkWidgetPick( sceneNode, widgetTree, wasHighlightOver, tableView );
		} );
	}
}

}}} // namespace EE::UI::Tools
