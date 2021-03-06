/*============================================================================

The Medical Imaging Interaction Toolkit (MITK)

Copyright (c) German Cancer Research Center (DKFZ)
All rights reserved.

Use of this source code is governed by a 3-clause BSD license that can be
found in the LICENSE file.

============================================================================*/

#ifndef mitkUIntPropertySerializer_h_included
#define mitkUIntPropertySerializer_h_included

#include "mitkBasePropertySerializer.h"

#include "mitkProperties.h"

namespace mitk
{
  class UIntPropertySerializer : public BasePropertySerializer
  {
  public:
    mitkClassMacro(UIntPropertySerializer, BasePropertySerializer);
    itkFactorylessNewMacro(Self) itkCloneMacro(Self)

      TiXmlElement *Serialize() override
    {
      if (const UIntProperty *prop = dynamic_cast<const UIntProperty *>(m_Property.GetPointer()))
      {
        auto element = new TiXmlElement("unsigned");
        element->SetAttribute("value", static_cast<unsigned int>(prop->GetValue()));
        return element;
      }
      else
        return nullptr;
    }

    BaseProperty::Pointer Deserialize(TiXmlElement *element) override
    {
      if (!element)
        return nullptr;

      unsigned int integer;
      if (element->QueryUnsignedAttribute("value", &integer) == TIXML_SUCCESS)
      {
        return UIntProperty::New(integer).GetPointer();
      }
      else
      {
        return nullptr;
      }
    }

  protected:
    UIntPropertySerializer() {}
    ~UIntPropertySerializer() override {}
  };

} // namespace

// important to put this into the GLOBAL namespace (because it starts with 'namespace mitk')
MITK_REGISTER_SERIALIZER(UIntPropertySerializer);

#endif
