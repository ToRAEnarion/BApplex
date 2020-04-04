#include "treemultibutton.h"

#include <QPushButton>



TreeMultiButton::TreeMultiButton(QObject *parent) : QItemDelegate (parent)
{

}

QWidget* TreeMultiButton::createEditor( QWidget* parent, const QStyleOptionViewItem& option, const QModelIndex& index ) const
{
     if ( index.column() == 4 )
     {
          QPushButton* button = new QPushButton( "pioup",parent );
          button->setIcon( QIcon(":/BUG") );
          //button->setFixedSize( 16, 16 );
          //button->setAutoRaise( true );
          //button->setVisible( true );

       //   CONNECT( button, SIGNAL( clicked() ), this, SLOT( emitCommitData() ) );

          return button;
     }
     return ( new QWidget );
}
