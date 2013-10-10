/**
 * @file gomibako.c
 */

#include <sys/cdefs.h>
__FBSDID("$FreeBSD: release/9.1.0/sys/dev/null/null.c 230320 2012-01-18 21:54:34Z gnn $");

#include <sys/param.h>
#include <sys/systm.h>
#include <sys/conf.h>
#include <sys/uio.h>
#include <sys/kernel.h>
#include <sys/malloc.h>
#include <sys/module.h>
#include <sys/priv.h>
#include <sys/disk.h>
#include <sys/bus.h>
#include <sys/filio.h>

#include <machine/bus.h>
#include <machine/vmparam.h>

/* For use with destroy_dev(9). */
static struct cdev *gomibako_dev;

static d_write_t gomibako_write;
static d_ioctl_t gomibako_ioctl;

static struct cdevsw gomibako_cdevsw = {
	.d_version =	D_VERSION,
	.d_read =       (d_read_t *)nullop,
	.d_write =	gomibako_write,
	.d_ioctl =	gomibako_ioctl,
	.d_name =	"gomibako",
};

/* ARGSUSED */
static int
gomibako_write(struct cdev *dev __unused, struct uio *uio, int flags __unused)
{
	uio->uio_resid = 0;

	return (0);
}

/* ARGSUSED */
static int
gomibako_ioctl(struct cdev *dev __unused, u_long cmd, caddr_t data __unused,
    int flags __unused, struct thread *td)
{
	int error;
	error = 0;

	switch (cmd) {
	case DIOCSKERNELDUMP:
		error = priv_check(td, PRIV_SETDUMPER);
		if (error == 0)
			error = set_dumper(NULL);
		break;
	case FIONBIO:
		break;
	case FIOASYNC:
		if (*(int *)data != 0)
			error = EINVAL;
		break;
	default:
		error = ENOIOCTL;
	}
	return (error);
}

/* ARGSUSED */
static int
gomibako_modevent(module_t mod __unused, int type, void *data __unused)
{
	switch(type) {
	case MOD_LOAD:
		if (bootverbose)
			printf("gomibako: <gomibako device, zero device>\n");
		gomibako_dev = make_dev_credf(MAKEDEV_ETERNAL_KLD, &gomibako_cdevsw, 0,
		    NULL, UID_ROOT, GID_WHEEL, 0666, "gomibako");
		break;

	case MOD_UNLOAD:
		destroy_dev(gomibako_dev);
		break;

	case MOD_SHUTDOWN:
		break;

	default:
		return (EOPNOTSUPP);
	}

	return (0);
}

DEV_MODULE(gomibako, gomibako_modevent, NULL);
MODULE_VERSION(gomibako, 1);
