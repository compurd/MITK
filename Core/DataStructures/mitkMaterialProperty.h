#ifndef _MITK_MATERIAL_PROPERTY_H_
#define _MITK_MATERIAL_PROPERTY_H_

#include "mitkCommon.h"
#include "mitkBaseProperty.h"
#include <itkRGBPixel.h>
#include <itkObject.h>
#include <itkVectorContainer.h>
#include <vtkSystemIncludes.h>

namespace mitk
{
// forward declarations
class DataTreeNode;

/**
 * Encapsulates 3D visualization properties which are forwarded to vtk for
 * color mapping. This includes color, specular coefficient and power, opacity
 * interpolation type (flat, gouraud, phong) and representation (points, 
 * wireframe or surface).
 * @see vtkProperty
*/
class MaterialProperty : public BaseProperty
{
public:
    mitkClassMacro( MaterialProperty, BaseProperty );

    typedef itk::RGBPixel<vtkFloatingPointType> Color;
    
    enum InterpolationType
    {
        Flat, Gouraud, Phong
    };

    enum RepresentationType
    {
        Points, Wireframe, Surface
    };

    /**
     * Constructor. Materials are set to the following default values:
     * Color (0.5, 0.5, 0.5) color coefficient 1.0, specular color (1.0, 1.0, 1.0),
     * specular coefficient 1.0, specular power 10, opacity 1.0, interpolation
     * Gouraud, representation Surface.
     */
    MaterialProperty( mitk::DataTreeNode* node = NULL );

    /**
     * Constructor. All values besides the given ones are set to defaults as 
     * described in the default constructor
     * @param color the material color in RGB. Each RGB value should be in the
     *              range [0..1]
     * @param opacity the opacity of the material. 0.0 means fully transparent
     *              and 1.0 means solid.
     */
    MaterialProperty( Color color, vtkFloatingPointType opacity = 1.0f, mitk::DataTreeNode* node = NULL );

    /**
     * Constructor. All values besides the given ones are set to defaults as 
     * described in the default constructor
     * @param red the red component of the materials color (range [0..1])
     * @param green the green component of the materials color (range [0..1])
     * @param blue the blue component of the materials color (range [0..1])
     * @param opacity the opacity of the material. 0.0 means fully transparent
     *        and 1.0 means solid.
     */
    MaterialProperty( vtkFloatingPointType red, vtkFloatingPointType green, vtkFloatingPointType blue, vtkFloatingPointType opacity = 1.0f, mitk::DataTreeNode* node = NULL );

    /**
     * Constructor. All values besides the given ones are set to defaults as 
     * described in the default constructor
     * @param red the red component of the materials color (range [0..1])
     * @param green the green component of the materials color (range [0..1])
     * @param blue the blue component of the materials color (range [0..1])
     * @param colorCoefficient a scaling factor for the color coefficient which
     *        will be multiplied with each color component (range [0..1]).
     * @param specularCoefficient controls in combination with the specular power
     *        how shiny the material will appear (range [0..1]).
     * @param specularPower controls in combination with the specular coefficient
     *        how shiny the material will appear (range [0..inf]).
     * @param opacity the opacity of the material. 0.0 means fully transparent
     *        and 1.0 means solid.
     */
    MaterialProperty( vtkFloatingPointType red, vtkFloatingPointType green, vtkFloatingPointType blue, vtkFloatingPointType colorCoefficient, 
      vtkFloatingPointType specularCoefficient, vtkFloatingPointType specularPower, vtkFloatingPointType opacity, mitk::DataTreeNode* node = NULL );

    /**
     * Constructor. All values besides the given ones are set to defaults as 
     * described in the default constructor
     * 
     * @param color the material color in RGB. Each RGB value should be in the
     *        range [0..1]
     * @param colorCoefficient a scaling factor for the color coefficient which
     *        will be multiplied with each color component (range [0..1]).
     * @param specularCoefficient controls in combination with the specular power
     *        how shiny the material will appear (range [0..1]).
     * @param specularPower controls in combination with the specular coefficient
     *        how shiny the material will appear (range [0..inf]).
     * @param opacity the opacity of the material. 0.0 means fully transparent
     *        and 1.0 means solid.
     */
    MaterialProperty( Color color, vtkFloatingPointType colorCoefficient, vtkFloatingPointType specularCoefficient, vtkFloatingPointType specularPower, vtkFloatingPointType opacity, mitk::DataTreeNode* node = NULL );

    /**
     * Copy constructor
     */
    MaterialProperty( const MaterialProperty& property );

    
    /**
     * The material Property is associated with a data tree node, which may be
     * set via this function. If node is != NULL, each call to the set/get functions
     * is forwarded to the node. If node is NULL, the local member variables are
     * used to store/retrieve the values;
     * @param node the data tree node associated with the given material property
     */
    virtual void SetDataTreeNode( mitk::DataTreeNode* node );
    
    /**
     * Returns the data tree node associated with the current material property
     * @returns the data tree node associated with the current matieral property.
     *          NOte: this may be NULL. If the value returned is NULL, then the local
     *          member variables are used for storing the properties
     */
    virtual mitk::DataTreeNode* GetDataTreeNode() const;
    
    /**
     * Sets the materials color in RGB space. The rgb components have to be
     * in the range [0..1]
     * @param color the new color of the material
     */
    virtual void SetColor( Color color );

    /**
     * Sets the materials color in RGB space. The rgb components have to be
     * in the range [0..1]
     * @param red the red component of the materials color (range [0..1])
     * @param green the green component of the materials color (range [0..1])
     * @param blue the blue component of the materials color (range [0..1])
     */
    virtual void SetColor( vtkFloatingPointType red, vtkFloatingPointType green, vtkFloatingPointType blue );

    /**
     * Sets a attenuation coefficient for the color. A value of 0 results in 
     * a black object. VAlid range is [0..1]
     * @param coefficient the color attenuation coefficient
     */
    virtual void SetColorCoefficient( vtkFloatingPointType coefficient );

    /**
     * Sets the specular color
     * @param color the specular color in RGB. Each RGB value should be in the
     *        range [0..1]
     */
    virtual void SetSpecularColor( Color color );

    /**
     * Sets the specular color
     * @param red the red component of the specular color (range [0..1])
     * @param green the green component of the specular color (range [0..1])
     * @param blue the blue component of the specular color (range [0..1])
     */
    virtual void SetSpecularColor( vtkFloatingPointType red, vtkFloatingPointType green, vtkFloatingPointType blue );

    /**
     * Sets the specular coefficient which controls the shininess of the object
     * together with the specular power
     * @param specularCoefficient the new specular coefficient. Valid range
     *        is [0..1]
     */
    virtual void SetSpecularCoefficient( vtkFloatingPointType specularCoefficient );

    /**
     * Sets the specular power which controls the shininess of the object
     * together with the specular coefficient
     * @param specularCoefficient the new specular coefficient. Valid range
     *        is [0..inf]
     */
    virtual void SetSpecularPower( vtkFloatingPointType specularPower );

    /**
     * Sets the opacity of the material, which controls how transparent the 
     * object appears. Valid range is [0..1], where 0 means fully transparent
     * and 1 means a solid surface.
     * @param opacity the new opacity of the material
     */
    virtual void SetOpacity( vtkFloatingPointType opacity );

    /**
     * Sets the surface interpolation method of the object rendered using the
     * given materials. Valid Interopation types are Flat, Gouraud and Phong.
     * See any computer graphics book for their meaning
     * @param interpolation the interpolation method used for rendering of
     *        surfaces.
     */
    virtual void SetInterpolation( InterpolationType interpolation );

    /**
     * Sets the surface representation method of the object rendered using the
     * given materials. Valid Interopation types are Points, Wireframe and
     * Surface.
     * @param representation the representation method used for rendering of
     *        surfaces.
     */
    virtual void SetRepresentation( RepresentationType representation );

    /**
     * @returns the color of the material
     */
    virtual Color GetColor() const;

    /**
     * @returns the color coefficient of the material. Range is [0..1]
     */
    virtual vtkFloatingPointType GetColorCoefficient() const;

    /**
     * @returns the specular color of the material in rgb values, which 
     * range from 0 .. 1
     */
    virtual Color GetSpecularColor() const;

    /**
     * @returns the specular coefficient used for rendering. Range is [0..1]
     */
    virtual vtkFloatingPointType GetSpecularCoefficient() const;

    /**
     * @returns the specular power. Ranges from 0 to infinity
     */
    virtual vtkFloatingPointType GetSpecularPower() const;

    /**
     * @returns the opacity of the material. Ranges from 0 to 1
     */
    virtual vtkFloatingPointType GetOpacity() const;

    /**
     * @returns the interpolation method used for rendering.
     */
    virtual InterpolationType GetInterpolation() const;

    /**
     * @returns the representation type used for rendering.
     */
    virtual RepresentationType GetRepresentation() const;

    /**
     * @returns the interpolation method used for rendering using the predefined
     * vtk constants.
     */
    virtual int GetVtkInterpolation() const;

    /**
     * @returns the representation type used for rendering using the predefined
     * vtk constants.
     */
    virtual int GetVtkRepresentation() const;

    /**
     * Fills the current materials with the properties of the
     * given material.
     * @param property the materials which should be copied in the
     *        current materials
     */
    virtual void Initialize( const MaterialProperty& property );

    /**
     * comparison operator which uses the member variables for
     * comparison
     */
    virtual bool operator==( const BaseProperty& property ) const;

protected:

    Color m_Color;

    Color m_SpecularColor;

    vtkFloatingPointType m_ColorCoefficient;

    vtkFloatingPointType m_SpecularCoefficient;

    vtkFloatingPointType m_SpecularPower;

    vtkFloatingPointType m_Opacity;

    InterpolationType m_Interpolation;

    RepresentationType m_Representation;
    
    mitk::DataTreeNode* m_DataTreeNode;
    
    static const char* COLOR_KEY;     
    static const char* SPECULAR_COLOR_KEY;     
    static const char* COLOR_COEFFICIENT_KEY; 
    static const char* SPECULAR_COEFFICIENT_KEY; 
    static const char* SPECULAR_POWER_KEY; 
    static const char* OPACITY_KEY; 
    static const char* INTERPOLATION_KEY; 
    static const char* REPRESENTATION_KEY; 

};

typedef itk::VectorContainer< unsigned int, MaterialProperty* > MaterialPropertyVectorContainer;

}


#endif
