#pragma once

#include  "QsSdkConfig.h"
#include  <string>
#include  <vector>
#include  <algorithm>
class QS_SDK_API QsBaseType{
public:
	QsBaseType(){};

	virtual ~QsBaseType() {};

    inline  bool IsPropHasBeenSet(std::string strPropName)
    {
        auto it = std::find( m_propsHasBeenSet.begin(), m_propsHasBeenSet.end(), strPropName );
        if( it != m_propsHasBeenSet.end() ) // finded
        {
            return true;
        }
        else
        {
            return false;
        }
    }

    virtual bool CheckIfInputIsVaild(){ return true;};
protected:
    std::vector<std::string> m_propsHasBeenSet;
};
