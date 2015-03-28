#ifndef LIBRARIES_POLYMORPH_H
#define LIBRARIES_POLYMORPH_H

#ifdef __cplusplus
#ifdef __USE_AMIGAOS_NAMESPACE__
namespace AmigaOS {
#endif
extern "C" {
#endif

#define POLYMORPH_NAME		"polymorph.library"
#define	POLYMORPH_API_V		1
#define	POLYMORPH_API_R		0

struct PolymorphApplication
{
    struct Node Node;
	struct ExecIFace *IExec;
	struct GraphicsIFace *IGfx;
	struct IntuitionIFace *IIntuition;
	struct DOSIFace *IDOS;
	struct UtilityIFace *IUtility;
	struct LocaleIFace *ILocale;
	struct PolymorphIFace *IPolymorph;
	struct ProcessorIFace *IProcessor;
};

#ifdef __cplusplus
}
#ifdef __USE_AMIGAOS_NAMESPACE__
}
#endif
#endif

#endif /* LIBRARIES_POLYMORPH_H */
