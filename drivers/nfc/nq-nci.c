<<<<<<< HEAD
/* Copyright (c) 2015, The Linux Foundation. All rights reserved.
=======
/* Copyright (c) 2015-2016, The Linux Foundation. All rights reserved.
>>>>>>> FETCH_HEAD
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 and
 * only version 2 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/reboot.h>
#include <linux/slab.h>
#include <linux/irq.h>
#include <linux/delay.h>
#include <linux/interrupt.h>
#include <linux/gpio.h>
#include <linux/spinlock.h>
#include <linux/of_gpio.h>
#include <linux/of_device.h>
#include <linux/uaccess.h>
#include "nq-nci.h"
<<<<<<< HEAD
#include <linux/dma-mapping.h>
#include <linux/dmapool.h>
=======
#include <linux/clk.h>
>>>>>>> FETCH_HEAD
#ifdef CONFIG_COMPAT
#include <linux/compat.h>
#endif

struct nqx_platform_data {
	unsigned int irq_gpio;
	unsigned int en_gpio;
	unsigned int clkreq_gpio;
	unsigned int firm_gpio;
<<<<<<< HEAD
=======
	unsigned int ese_gpio;
>>>>>>> FETCH_HEAD
	const char *clk_src_name;
};

static struct of_device_id msm_match_table[] = {
	{.compatible = "qcom,nq-nci"},
	{}
};

MODULE_DEVICE_TABLE(of, msm_match_table);
<<<<<<< HEAD
#define MAX_BUFFER_SIZE			(320)
#define PACKET_MAX_LENGTH		(258)
/* Read data */
#define PACKET_HEADER_SIZE_NCIPACKET_HEADER_SIZE_NCI	(4)
#define MAX_PACKET_SIZE			(PACKET_HEADER_SIZE_NCI + 255)
/* will timeout in approx. 100ms as 10us steps */
#define NTF_TIMEOUT				(100)
#define	CORE_RESET_RSP_GID		(0x60)
#define	CORE_RESET_OID			(0x00)
#define CORE_RST_NTF_LENGTH		(0x02)
=======

#define MAX_BUFFER_SIZE			(320)
>>>>>>> FETCH_HEAD
#define WAKEUP_SRC_TIMEOUT		(2000)

struct nqx_dev {
	wait_queue_head_t	read_wq;
	struct	mutex		read_mutex;
	struct	i2c_client	*client;
	struct	miscdevice	nqx_device;
<<<<<<< HEAD
=======
	/* NFC GPIO variables */
>>>>>>> FETCH_HEAD
	unsigned int		irq_gpio;
	unsigned int		en_gpio;
	unsigned int		firm_gpio;
	unsigned int		clkreq_gpio;
<<<<<<< HEAD
	/* NFC_IRQ state */
	bool			irq_enabled;
	spinlock_t		irq_enabled_lock;
	unsigned int		count_irq;
	/* CLK control */
	unsigned int		core_reset_ntf;
	bool			clk_run;
	struct dma_pool *nfc_dma_pool;
	dma_addr_t dma_handle_physical_addr;
	void *dma_virtual_addr;
=======
	unsigned int		ese_gpio;
	/* NFC VEN pin state powered by Nfc */
	bool			nfc_ven_enabled;
	/* NFC_IRQ state */
	bool			irq_enabled;
	/* NFC_IRQ wake-up state */
	bool			irq_wake_up;
	spinlock_t		irq_enabled_lock;
	unsigned int		count_irq;
	/* Initial CORE RESET notification */
	unsigned int		core_reset_ntf;
	/* CLK control */
	bool			clk_run;
	struct	clk		*s_clk;
	/* read buffer*/
	size_t kbuflen;
	u8 *kbuf;
	struct nqx_platform_data *pdata;
>>>>>>> FETCH_HEAD
};

static int nfcc_reboot(struct notifier_block *notifier, unsigned long val,
			void *v);
<<<<<<< HEAD

=======
/*clock enable function*/
static int nqx_clock_select(struct nqx_dev *nqx_dev);
/*clock disable function*/
static int nqx_clock_deselect(struct nqx_dev *nqx_dev);
>>>>>>> FETCH_HEAD
static struct notifier_block nfcc_notifier = {
	.notifier_call	= nfcc_reboot,
	.next			= NULL,
	.priority		= 0
};

unsigned int	disable_ctrl;

static void nqx_init_stat(struct nqx_dev *nqx_dev)
{
	nqx_dev->count_irq = 0;
}

static void nqx_disable_irq(struct nqx_dev *nqx_dev)
{
	unsigned long flags;

	spin_lock_irqsave(&nqx_dev->irq_enabled_lock, flags);
	if (nqx_dev->irq_enabled) {
		disable_irq_nosync(nqx_dev->client->irq);
		nqx_dev->irq_enabled = false;
	}
	spin_unlock_irqrestore(&nqx_dev->irq_enabled_lock, flags);
}

<<<<<<< HEAD
=======
/**
 * nqx_enable_irq()
 *
 * Check if interrupt is enabled or not
 * and enable interrupt
 *
 * Return: void
 */
>>>>>>> FETCH_HEAD
static void nqx_enable_irq(struct nqx_dev *nqx_dev)
{
	unsigned long flags;

	spin_lock_irqsave(&nqx_dev->irq_enabled_lock, flags);
	if (!nqx_dev->irq_enabled) {
		nqx_dev->irq_enabled = true;
		enable_irq(nqx_dev->client->irq);
	}
	spin_unlock_irqrestore(&nqx_dev->irq_enabled_lock, flags);
}

static irqreturn_t nqx_dev_irq_handler(int irq, void *dev_id)
{
	struct nqx_dev *nqx_dev = dev_id;
	unsigned long flags;
<<<<<<< HEAD
	int ret;

	if (device_may_wakeup(&nqx_dev->client->dev) &&
		(nqx_dev->client->dev.power.is_suspended == true)) {
		pm_wakeup_event(&nqx_dev->client->dev, WAKEUP_SRC_TIMEOUT);
	}
	ret = gpio_get_value_cansleep(nqx_dev->irq_gpio);
	if (!ret) {
		dev_info(&nqx_dev->client->dev,
			"nqx nfc : nqx_dev_irq_handler error = %d\n", ret);
		return IRQ_HANDLED;
	}

=======

	if (device_may_wakeup(&nqx_dev->client->dev))
		pm_wakeup_event(&nqx_dev->client->dev, WAKEUP_SRC_TIMEOUT);

	nqx_disable_irq(nqx_dev);
>>>>>>> FETCH_HEAD
	spin_lock_irqsave(&nqx_dev->irq_enabled_lock, flags);
	nqx_dev->count_irq++;
	spin_unlock_irqrestore(&nqx_dev->irq_enabled_lock, flags);
	wake_up(&nqx_dev->read_wq);

	return IRQ_HANDLED;
}

static ssize_t nfc_read(struct file *filp, char __user *buf,
					size_t count, loff_t *offset)
{
	struct nqx_dev *nqx_dev = filp->private_data;
	unsigned char *tmp = NULL;
	int ret;
	int irq_gpio_val = 0;

<<<<<<< HEAD
	if (count > MAX_BUFFER_SIZE)
		count = MAX_BUFFER_SIZE;
=======
	if (!nqx_dev) {
		ret = -ENODEV;
		goto out;
	}

	if (count > nqx_dev->kbuflen)
		count = nqx_dev->kbuflen;
>>>>>>> FETCH_HEAD

	dev_dbg(&nqx_dev->client->dev, "%s : reading %zu bytes.\n",
			__func__, count);

	mutex_lock(&nqx_dev->read_mutex);

<<<<<<< HEAD
	irq_gpio_val = gpio_get_value_cansleep(nqx_dev->irq_gpio);
=======
	irq_gpio_val = gpio_get_value(nqx_dev->irq_gpio);
>>>>>>> FETCH_HEAD
	if (irq_gpio_val == 0) {
		if (filp->f_flags & O_NONBLOCK) {
			dev_err(&nqx_dev->client->dev,
			":f_falg has O_NONBLOCK. EAGAIN\n");
			ret = -EAGAIN;
			goto err;
		}
<<<<<<< HEAD
		nqx_dev->irq_enabled = true;
		enable_irq(nqx_dev->client->irq);
		if (gpio_get_value_cansleep(nqx_dev->irq_gpio)) {
			nqx_disable_irq(nqx_dev);
		} else {
			ret = wait_event_interruptible(nqx_dev->read_wq,
					gpio_get_value(nqx_dev->irq_gpio));
			nqx_disable_irq(nqx_dev);
			if (ret)
				goto err;
		}
	}

	/* Read data */
	tmp = nqx_dev->dma_virtual_addr;
	memset(tmp, 0x00, MAX_BUFFER_SIZE);
	ret = i2c_master_recv(nqx_dev->client, tmp, count);

	mutex_unlock(&nqx_dev->read_mutex);

	if (ret < 0) {
		dev_err(&nqx_dev->client->dev,
			"%s: i2c_master_recv returned %d\n", __func__, ret);
		return ret;
=======
		while (1) {
			ret = 0;
			if (!nqx_dev->irq_enabled) {
				nqx_dev->irq_enabled = true;
				enable_irq(nqx_dev->client->irq);
			}
			if (!gpio_get_value(nqx_dev->irq_gpio)) {
				ret = wait_event_interruptible(nqx_dev->read_wq,
					!nqx_dev->irq_enabled);
			}
			if (ret)
				goto err;
			nqx_disable_irq(nqx_dev);

			if (gpio_get_value(nqx_dev->irq_gpio))
				break;
			dev_err_ratelimited(&nqx_dev->client->dev, "gpio is low, no need to read data\n");
		}
	}

	tmp = nqx_dev->kbuf;
	if (!tmp) {
		dev_err(&nqx_dev->client->dev,
			"%s: device doesn't exist anymore\n", __func__);
		ret = -ENODEV;
		goto err;
	}
	memset(tmp, 0x00, count);

	/* Read data */
	ret = i2c_master_recv(nqx_dev->client, tmp, count);
	if (ret < 0) {
		dev_err(&nqx_dev->client->dev,
			"%s: i2c_master_recv returned %d\n", __func__, ret);
		goto err;
>>>>>>> FETCH_HEAD
	}
	if (ret > count) {
		dev_err(&nqx_dev->client->dev,
			"%s: received too many bytes from i2c (%d)\n",
			__func__, ret);
<<<<<<< HEAD
		return -EIO;
	}
	if (copy_to_user(buf, tmp, ret)) {
		dev_warn(&nqx_dev->client->dev,
			"%s : failed to copy to user space\n", __func__);
		return -EFAULT;
	}
=======
		ret = -EIO;
		goto err;
	}
#ifdef NFC_KERNEL_BU
		dev_dbg(&nqx_dev->client->dev, "%s : NfcNciRx %x %x %x\n",
			__func__, tmp[0], tmp[1], tmp[2]);
#endif
	if (copy_to_user(buf, tmp, ret)) {
		dev_warn(&nqx_dev->client->dev,
			"%s : failed to copy to user space\n", __func__);
		ret = -EFAULT;
		goto err;
	}
	mutex_unlock(&nqx_dev->read_mutex);
>>>>>>> FETCH_HEAD
	return ret;

err:
	mutex_unlock(&nqx_dev->read_mutex);
<<<<<<< HEAD
=======
out:
>>>>>>> FETCH_HEAD
	return ret;
}

static ssize_t nfc_write(struct file *filp, const char __user *buf,
				size_t count, loff_t *offset)
{
	struct nqx_dev *nqx_dev = filp->private_data;
<<<<<<< HEAD
	char tmp[MAX_BUFFER_SIZE];
	int ret = 0;
	if (count > MAX_BUFFER_SIZE) {
		dev_err(&nqx_dev->client->dev, "%s: out of memory\n",
			__func__);
		return -ENOMEM;
	}
	if (copy_from_user(tmp, buf, count)) {
		dev_err(&nqx_dev->client->dev,
			"%s: failed to copy from user space\n", __func__);
		return -EFAULT;
	}
=======
	char *tmp = NULL;
	int ret = 0;

	if (!nqx_dev) {
		ret = -ENODEV;
		goto out;
	}
	if (count > nqx_dev->kbuflen) {
		dev_err(&nqx_dev->client->dev, "%s: out of memory\n",
			__func__);
		ret = -ENOMEM;
		goto out;
	}

	tmp = memdup_user(buf, count);
	if (IS_ERR(tmp)) {
		dev_err(&nqx_dev->client->dev, "%s: memdup_user failed\n",
			__func__);
		ret = PTR_ERR(tmp);
		goto out;
	}

>>>>>>> FETCH_HEAD
	ret = i2c_master_send(nqx_dev->client, tmp, count);
	if (ret != count) {
		dev_err(&nqx_dev->client->dev,
		"%s: failed to write %d\n", __func__, ret);
		ret = -EIO;
<<<<<<< HEAD
	}
	dev_dbg(&nqx_dev->client->dev, "%s : NfcNciTx %x %x %x\n",
			__func__, tmp[0], tmp[1], tmp[2]);
	usleep_range(1000, 1100);
	return ret;
}

=======
		goto out_free;
	}
#ifdef NFC_KERNEL_BU
	dev_dbg(&nqx_dev->client->dev,
			"%s : i2c-%d: NfcNciTx %x %x %x\n",
			__func__, iminor(file_inode(filp)),
			tmp[0], tmp[1], tmp[2]);
#endif
	usleep_range(1000, 1100);
out_free:
	kfree(tmp);
out:
	return ret;
}

/*
	Power management of the eSE
	NFC & eSE ON : NFC_EN high and eSE_pwr_req high.
	NFC OFF & eSE ON : NFC_EN high and eSE_pwr_req high.
	NFC OFF & eSE OFF : NFC_EN low and eSE_pwr_req low.
*/
static int nqx_ese_pwr(struct nqx_dev *nqx_dev, unsigned long int arg)
{
	int r = -1;

	/* Let's store the NFC_EN pin state*/
	if (arg == 0) {
		/* We want to power on the eSE and to do so we need the
		 * eSE_pwr_req pin and the NFC_EN pin to be high
		 */
		nqx_dev->nfc_ven_enabled = gpio_get_value(nqx_dev->en_gpio);
		if (!nqx_dev->nfc_ven_enabled) {
			gpio_set_value(nqx_dev->en_gpio, 1);
			/* hardware dependent delay */
			usleep_range(1000, 1100);
		}
		if (gpio_is_valid(nqx_dev->ese_gpio)) {
			if (gpio_get_value(nqx_dev->ese_gpio)) {
				dev_dbg(&nqx_dev->client->dev, "ese_gpio is already high\n");
				r = 0;
			} else {
				gpio_set_value(nqx_dev->ese_gpio, 1);
				if (gpio_get_value(nqx_dev->ese_gpio)) {
					dev_dbg(&nqx_dev->client->dev, "ese_gpio is enabled\n");
					r = 0;
				}
			}
		}
	} else if (arg == 1) {
		if (gpio_is_valid(nqx_dev->ese_gpio)) {
			gpio_set_value(nqx_dev->ese_gpio, 0);
			if (!gpio_get_value(nqx_dev->ese_gpio)) {
				dev_dbg(&nqx_dev->client->dev, "ese_gpio is disabled\n");
				r = 0;
			}
		}
		if (!nqx_dev->nfc_ven_enabled) {
			/* hardware dependent delay */
			usleep_range(1000, 1100);
			dev_dbg(&nqx_dev->client->dev, "disabling en_gpio\n");
			gpio_set_value(nqx_dev->en_gpio, 0);
		}
	} else if (arg == 3) {
		if (!nqx_dev->nfc_ven_enabled)
			r = 0;
		else {
			if (gpio_is_valid(nqx_dev->ese_gpio))
				r = gpio_get_value(nqx_dev->ese_gpio);
		}
	}
	return r;
}

>>>>>>> FETCH_HEAD
static int nfc_open(struct inode *inode, struct file *filp)
{
	int ret = 0;
	struct nqx_dev *nqx_dev = container_of(filp->private_data,
				struct nqx_dev, nqx_device);

	filp->private_data = nqx_dev;
	nqx_init_stat(nqx_dev);
<<<<<<< HEAD
	/* Enable interrupts from NFCC NFC_INT new NCI data available */
	nqx_enable_irq(nqx_dev);
=======
>>>>>>> FETCH_HEAD

	dev_dbg(&nqx_dev->client->dev,
			"%s: %d,%d\n", __func__, imajor(inode), iminor(inode));
	return ret;
}

<<<<<<< HEAD
/*
 * Inside nfc_ioctl_power_states
 *
 * @brief	ioctl functions
 *
 *
 * Device control
 * remove control via ioctl
=======
/**
 * nfc_ioctl_power_states() - power control
 * @filp:	pointer to the file descriptor
 * @arg:	mode that we want to move to
 *
 * Device power control. Depending on the arg value, device moves to
 * different states
>>>>>>> FETCH_HEAD
 * (arg = 0): NFC_ENABLE	GPIO = 0, FW_DL GPIO = 0
 * (arg = 1): NFC_ENABLE	GPIO = 1, FW_DL GPIO = 0
 * (arg = 2): FW_DL GPIO = 1
 *
<<<<<<< HEAD
 *
 */
int nfc_ioctl_power_states(struct file *filp, unsigned int cmd,
							unsigned long arg)
=======
 * Return: -ENOIOCTLCMD if arg is not supported, 0 in any other case
 */
int nfc_ioctl_power_states(struct file *filp, unsigned long arg)
>>>>>>> FETCH_HEAD
{
	int r = 0;
	struct nqx_dev *nqx_dev = filp->private_data;

	if (arg == 0) {
		/* We are attempting a hardware reset so let us disable
		 * interrupts to avoid spurious notifications to upper
		 * layers.
		 */
		nqx_disable_irq(nqx_dev);
		dev_dbg(&nqx_dev->client->dev,
			"gpio_set_value disable: %s: info: %p\n",
			__func__, nqx_dev);
		if (gpio_is_valid(nqx_dev->firm_gpio))
			gpio_set_value(nqx_dev->firm_gpio, 0);
<<<<<<< HEAD
		gpio_set_value(nqx_dev->en_gpio, 0);
		/* hardware dependent delay */
		msleep(100);
	} else if (arg == 1) {
=======

		if (gpio_is_valid(nqx_dev->ese_gpio)) {
			if (!gpio_get_value(nqx_dev->ese_gpio)) {
				dev_dbg(&nqx_dev->client->dev, "disabling en_gpio\n");
				gpio_set_value(nqx_dev->en_gpio, 0);
			} else {
				dev_dbg(&nqx_dev->client->dev, "keeping en_gpio high\n");
			}
		} else {
			dev_dbg(&nqx_dev->client->dev, "ese_gpio invalid, set en_gpio to low\n");
			gpio_set_value(nqx_dev->en_gpio, 0);
		}
		r = nqx_clock_deselect(nqx_dev);
		if (r < 0)
			dev_err(&nqx_dev->client->dev, "unable to disable clock\n");
		nqx_dev->nfc_ven_enabled = false;
		/* hardware dependent delay */
		msleep(100);
	} else if (arg == 1) {
		nqx_enable_irq(nqx_dev);
>>>>>>> FETCH_HEAD
		dev_dbg(&nqx_dev->client->dev,
			"gpio_set_value enable: %s: info: %p\n",
			__func__, nqx_dev);
		if (gpio_is_valid(nqx_dev->firm_gpio))
			gpio_set_value(nqx_dev->firm_gpio, 0);
		gpio_set_value(nqx_dev->en_gpio, 1);
<<<<<<< HEAD
		msleep(100);
=======
		r = nqx_clock_select(nqx_dev);
		if (r < 0)
			dev_err(&nqx_dev->client->dev, "unable to enable clock\n");
		nqx_dev->nfc_ven_enabled = true;
		msleep(20);
>>>>>>> FETCH_HEAD
	} else if (arg == 2) {
		/* We are switching to Dowload Mode, toggle the enable pin
		 * in order to set the NFCC in the new mode
		 */
<<<<<<< HEAD
=======
		if (gpio_is_valid(nqx_dev->ese_gpio)) {
			if (gpio_get_value(nqx_dev->ese_gpio)) {
				dev_err(&nqx_dev->client->dev, "FW download forbidden while ese is on\n");
				return -EBUSY; /* Device or resource busy */
			}
		}
>>>>>>> FETCH_HEAD
		gpio_set_value(nqx_dev->en_gpio, 1);
		msleep(20);
		if (gpio_is_valid(nqx_dev->firm_gpio))
			gpio_set_value(nqx_dev->firm_gpio, 1);
		msleep(20);
		gpio_set_value(nqx_dev->en_gpio, 0);
		msleep(100);
		gpio_set_value(nqx_dev->en_gpio, 1);
		msleep(20);
	} else {
		r = -ENOIOCTLCMD;
	}
<<<<<<< HEAD
=======

>>>>>>> FETCH_HEAD
	return r;
}

#ifdef CONFIG_COMPAT
static long nfc_compat_ioctl(struct file *pfile, unsigned int cmd,
				unsigned long arg)
{
	long r = 0;
	arg = (compat_u64)arg;

	switch (cmd) {
	case NFC_SET_PWR:
<<<<<<< HEAD
		nfc_ioctl_power_states(pfile, cmd, arg);
=======
		nfc_ioctl_power_states(pfile, arg);
		break;
	case ESE_SET_PWR:
		nqx_ese_pwr(pfile->private_data, arg);
		break;
	case ESE_GET_PWR:
		nqx_ese_pwr(pfile->private_data, 3);
>>>>>>> FETCH_HEAD
		break;
	case SET_RX_BLOCK:
		break;
	case SET_EMULATOR_TEST_POINT:
		break;
	default:
		r = -ENOTTY;
	}
	return r;
}
#endif

<<<<<<< HEAD
/*
 * Inside nfc_ioctl_core_reset_ntf
 *
 * @brief	nfc_ioctl_core_reset_ntf
 *
 * Allows callers to determine if a CORE_RESET_NTF has arrived
 *
 * Returns the value of variable core_reset_ntf
 *
 */
int nfc_ioctl_core_reset_ntf(struct file *filp, unsigned int cmd,
				unsigned long arg)
=======
/**
 * nfc_ioctl_core_reset_ntf()
 * @filp:       pointer to the file descriptor
 *
 * Allows callers to determine if a CORE_RESET_NTF has arrived
 *
 * Return: the value of variable core_reset_ntf
 */
int nfc_ioctl_core_reset_ntf(struct file *filp)
>>>>>>> FETCH_HEAD
{
	struct nqx_dev *nqx_dev = filp->private_data;
	dev_dbg(&nqx_dev->client->dev, "%s: returning = %d\n", __func__,
		nqx_dev->core_reset_ntf);
	return nqx_dev->core_reset_ntf;
}

static long nfc_ioctl(struct file *pfile, unsigned int cmd,
			unsigned long arg)
{
	int r = 0;

	switch (cmd) {
	case NFC_SET_PWR:
<<<<<<< HEAD
		r = nfc_ioctl_power_states(pfile, cmd, arg);
		break;
	case NFC_CLK_REQ:
=======
		r = nfc_ioctl_power_states(pfile, arg);
		break;
	case ESE_SET_PWR:
		r = nqx_ese_pwr(pfile->private_data, arg);
		break;
	case ESE_GET_PWR:
		r = nqx_ese_pwr(pfile->private_data, 3);
>>>>>>> FETCH_HEAD
		break;
	case SET_RX_BLOCK:
		break;
	case SET_EMULATOR_TEST_POINT:
		break;
	case NFCC_INITIAL_CORE_RESET_NTF:
<<<<<<< HEAD
		r = nfc_ioctl_core_reset_ntf(pfile, cmd, arg);
=======
		r = nfc_ioctl_core_reset_ntf(pfile);
>>>>>>> FETCH_HEAD
		break;
	default:
		r = -ENOIOCTLCMD;
	}
	return r;
}

static const struct file_operations nfc_dev_fops = {
	.owner = THIS_MODULE,
	.llseek = no_llseek,
	.read  = nfc_read,
	.write = nfc_write,
	.open = nfc_open,
	.unlocked_ioctl = nfc_ioctl,
#ifdef CONFIG_COMPAT
	.compat_ioctl = nfc_compat_ioctl
#endif
};

<<<<<<< HEAD
=======
/* Check for availability of NQ_ NFC controller hardware */
static int nfcc_hw_check(struct i2c_client *client, unsigned int enable_gpio)
{
	int ret = 0;

	unsigned char raw_nci_reset_cmd[] =  {0x20, 0x00, 0x01, 0x00};
	unsigned char nci_reset_rsp[6];

	/* making sure that the NFCC starts in a clean state. */
	gpio_set_value(enable_gpio, 0);/* ULPM: Disable */
	/* hardware dependent delay */
	msleep(20);
	gpio_set_value(enable_gpio, 1);/* HPD : Enable*/
	/* hardware dependent delay */
	msleep(20);

	/* send NCI CORE RESET CMD with Keep Config parameters */
	ret = i2c_master_send(client, raw_nci_reset_cmd,
						sizeof(raw_nci_reset_cmd));
	if (ret < 0) {
		dev_err(&client->dev,
		"%s: - i2c_master_send Error\n", __func__);
		goto err_nfcc_hw_check;
	}
	/* hardware dependent delay */
	msleep(30);

	/* Read Response of RESET command */
	ret = i2c_master_recv(client, nci_reset_rsp,
						sizeof(nci_reset_rsp));
	dev_err(&client->dev,
		"%s: - nq - reset cmd answer : NfcNciRx %x %x %x\n",
			__func__, nci_reset_rsp[0],
			nci_reset_rsp[1], nci_reset_rsp[2]);
	if (ret < 0) {
		dev_err(&client->dev,
		"%s: - i2c_master_recv Error\n", __func__);
		goto err_nfcc_hw_check;
	}
	gpio_set_value(enable_gpio, 0);/* ULPM: Disable */
	ret = 0;
	goto done;

err_nfcc_hw_check:
	ret = -ENXIO;
	dev_err(&client->dev,
		"%s: - NFCC HW not available\n", __func__);
done:
	return ret;
}

/*
	Routine to enable clock.
	this routine can be extended to select from multiple
	sources based on clk_src_name.
*/
static int nqx_clock_select(struct nqx_dev *nqx_dev)
{
	int r = 0;

	nqx_dev->s_clk =
			clk_get(&nqx_dev->client->dev, "ref_clk");

	if (nqx_dev->s_clk == NULL)
		goto err_clk;

	if (nqx_dev->clk_run == false)
		r = clk_prepare_enable(nqx_dev->s_clk);

	if (r)
		goto err_clk;

	nqx_dev->clk_run = true;

	return r;

err_clk:
	r = -1;
	return r;
}
/*
	Routine to disable clocks
*/
static int nqx_clock_deselect(struct nqx_dev *nqx_dev)
{
	int r = -1;

	if (nqx_dev->s_clk != NULL) {
		if (nqx_dev->clk_run == true) {
			clk_disable_unprepare(nqx_dev->s_clk);
			nqx_dev->clk_run = false;
		}
		return 0;
	}
	return r;
}

>>>>>>> FETCH_HEAD
static int nfc_parse_dt(struct device *dev, struct nqx_platform_data *pdata)
{
	int r = 0;
	struct device_node *np = dev->of_node;

	pdata->en_gpio = of_get_named_gpio(np, "qcom,nq-ven", 0);
	if ((!gpio_is_valid(pdata->en_gpio)))
		return -EINVAL;
	disable_ctrl = pdata->en_gpio;

	pdata->irq_gpio = of_get_named_gpio(np, "qcom,nq-irq", 0);
	if ((!gpio_is_valid(pdata->irq_gpio)))
		return -EINVAL;

	pdata->firm_gpio = of_get_named_gpio(np, "qcom,nq-firm", 0);
	if (!gpio_is_valid(pdata->firm_gpio)) {
		dev_warn(dev,
			"FIRM GPIO <OPTIONAL> error getting from OF node\n");
		pdata->firm_gpio = -EINVAL;
	}

<<<<<<< HEAD
=======
	pdata->ese_gpio = of_get_named_gpio(np, "qcom,nq-esepwr", 0);
	if (!gpio_is_valid(pdata->ese_gpio)) {
		dev_warn(dev,
			"ese GPIO <OPTIONAL> error getting from OF node\n");
		pdata->ese_gpio = -EINVAL;
	}

>>>>>>> FETCH_HEAD
	r = of_property_read_string(np, "qcom,clk-src", &pdata->clk_src_name);

	pdata->clkreq_gpio = of_get_named_gpio(np, "qcom,nq-clkreq", 0);

	if (r)
		return -EINVAL;
	return r;
}

static inline int gpio_input_init(const struct device * const dev,
			const int gpio, const char * const gpio_name)
{
	int r = gpio_request(gpio, gpio_name);
	if (r) {
		dev_err(dev, "unable to request gpio [%d]\n", gpio);
		return r;
	}

	r = gpio_direction_input(gpio);
	if (r)
		dev_err(dev, "unable to set direction for gpio [%d]\n", gpio);

	return r;
}

static int nqx_probe(struct i2c_client *client,
			const struct i2c_device_id *id)
{
	int r = 0;
	int irqn = 0;
	struct nqx_platform_data *platform_data;
	struct nqx_dev *nqx_dev;

	dev_dbg(&client->dev, "%s: enter\n", __func__);
	if (client->dev.of_node) {
		platform_data = devm_kzalloc(&client->dev,
			sizeof(struct nqx_platform_data), GFP_KERNEL);
		if (!platform_data) {
<<<<<<< HEAD
			dev_err(&client->dev,
			"%s: Failed to allocate memory\n", __func__);
			return -ENOMEM;
		}
		r = nfc_parse_dt(&client->dev, platform_data);
		if (r)
			return r;
	} else {
		platform_data = client->dev.platform_data;
	}
	dev_dbg(&client->dev,
		"%s, inside nfc-nci flags = %x\n",
		__func__, client->flags);
	if (platform_data == NULL) {
		dev_err(&client->dev, "%s: failed\n", __func__);
		return -ENODEV;
	}
	if (!i2c_check_functionality(client->adapter, I2C_FUNC_I2C)) {
		dev_err(&client->dev, "%s: need I2C_FUNC_I2C\n", __func__);
		return -ENODEV;
	}
	nqx_dev = kzalloc(sizeof(*nqx_dev), GFP_KERNEL);
	if (nqx_dev == NULL) {
		dev_err(&client->dev,
		"%s: failed to allocate memory for module data\n", __func__);
		return -ENOMEM;
	}
	nqx_dev->client = client;

	/* if coherent_dma_mask not set by the device, set it to ULONG_MAX */
	if (client->dev.coherent_dma_mask == 0)
		client->dev.coherent_dma_mask = ULONG_MAX;

		nqx_dev->nfc_dma_pool = NULL;
		nqx_dev->dma_virtual_addr = NULL;

		nqx_dev->nfc_dma_pool = dma_pool_create(
				"NFC-DMA", &client->dev,
				MAX_BUFFER_SIZE, 64, 4096);
		if (!nqx_dev->nfc_dma_pool) {
				dev_err(&client->dev,
				"nfc-nci probe: failed to allocate memory for dma_pool\n");
				r = -ENOMEM;
				goto err_free_dev;
		}

		nqx_dev->dma_virtual_addr = dma_pool_alloc(
				nqx_dev->nfc_dma_pool, GFP_KERNEL,
				&nqx_dev->dma_handle_physical_addr);
		if (!nqx_dev->dma_virtual_addr) {
				dev_err(&client->dev,
				"nfc-nci probe: failed to allocate coherent memory for i2c dma buffer\n");
				r = -ENOMEM;
				goto err_free_dev;
		}
=======
			r = -ENOMEM;
			goto err_platform_data;
		}
		r = nfc_parse_dt(&client->dev, platform_data);
		if (r)
			goto err_free_data;
	} else
		platform_data = client->dev.platform_data;

	dev_dbg(&client->dev,
		"%s, inside nfc-nci flags = %x\n",
		__func__, client->flags);

	if (platform_data == NULL) {
		dev_err(&client->dev, "%s: failed\n", __func__);
		r = -ENODEV;
		goto err_platform_data;
	}
	if (!i2c_check_functionality(client->adapter, I2C_FUNC_I2C)) {
		dev_err(&client->dev, "%s: need I2C_FUNC_I2C\n", __func__);
		r = -ENODEV;
		goto err_free_data;
	}
	nqx_dev = kzalloc(sizeof(*nqx_dev), GFP_KERNEL);
	if (nqx_dev == NULL) {
		r = -ENOMEM;
		goto err_free_data;
	}
	nqx_dev->client = client;
	nqx_dev->kbuflen = MAX_BUFFER_SIZE;
	nqx_dev->kbuf = kzalloc(MAX_BUFFER_SIZE, GFP_KERNEL);
	if (!nqx_dev->kbuf) {
		dev_err(&client->dev,
			"failed to allocate memory for nqx_dev->kbuf\n");
		r = -ENOMEM;
		goto err_free_dev;
	}
>>>>>>> FETCH_HEAD

	if (gpio_is_valid(platform_data->en_gpio)) {
		r = gpio_request(platform_data->en_gpio, "nfc_reset_gpio");
		if (r) {
			dev_err(&client->dev,
<<<<<<< HEAD
			"%s: unable to request gpio [%d]\n",
				__func__,
				platform_data->en_gpio);
			goto err_free_dev;
=======
			"%s: unable to request nfc reset gpio [%d]\n",
				__func__,
				platform_data->en_gpio);
			goto err_mem;
>>>>>>> FETCH_HEAD
		}
		r = gpio_direction_output(platform_data->en_gpio, 0);
		if (r) {
			dev_err(&client->dev,
<<<<<<< HEAD
				"%s: unable to set direction for gpio [%d]\n",
=======
				"%s: unable to set direction for nfc reset gpio [%d]\n",
>>>>>>> FETCH_HEAD
					__func__,
					platform_data->en_gpio);
			goto err_en_gpio;
		}
	} else {
<<<<<<< HEAD
		dev_err(&client->dev, "%s: dis gpio not provided\n", __func__);
		goto err_free_dev;
	}

	/* Register reboot notifier here */
	r = register_reboot_notifier(&nfcc_notifier);
	if (r) {
		dev_err(&client->dev,
			"%s: cannot register reboot notifier(err = %d)\n",
			__func__, r);
		goto err_en_gpio;
	}
	if (gpio_is_valid(platform_data->irq_gpio)) {
		r = gpio_request(platform_data->irq_gpio, "nfc_irq_gpio");
		if (r) {
			dev_err(&client->dev, "%s: unable to request gpio [%d]\n",
=======
		dev_err(&client->dev,
		"%s: nfc reset gpio not provided\n", __func__);
		goto err_mem;
	}

	if (gpio_is_valid(platform_data->irq_gpio)) {
		r = gpio_request(platform_data->irq_gpio, "nfc_irq_gpio");
		if (r) {
			dev_err(&client->dev, "%s: unable to request nfc irq gpio [%d]\n",
>>>>>>> FETCH_HEAD
				__func__, platform_data->irq_gpio);
			goto err_en_gpio;
		}
		r = gpio_direction_input(platform_data->irq_gpio);
		if (r) {
<<<<<<< HEAD

			dev_err(&client->dev,
			"%s: unable to set direction for gpio [%d]\n",
				__func__,
				platform_data->irq_gpio);
			goto err_irq;
=======
			dev_err(&client->dev,
			"%s: unable to set direction for nfc irq gpio [%d]\n",
				__func__,
				platform_data->irq_gpio);
			goto err_irq_gpio;
>>>>>>> FETCH_HEAD
		}
		irqn = gpio_to_irq(platform_data->irq_gpio);
		if (irqn < 0) {
			r = irqn;
<<<<<<< HEAD
			goto err_irq;
=======
			goto err_irq_gpio;
>>>>>>> FETCH_HEAD
		}
		client->irq = irqn;
	} else {
		dev_err(&client->dev, "%s: irq gpio not provided\n", __func__);
		goto err_en_gpio;
	}
	if (gpio_is_valid(platform_data->firm_gpio)) {
		r = gpio_request(platform_data->firm_gpio,
			"nfc_firm_gpio");
		if (r) {
			dev_err(&client->dev,
<<<<<<< HEAD
				"%s: unable to request gpio [%d]\n",
				__func__, platform_data->firm_gpio);
			goto err_irq;
=======
				"%s: unable to request nfc firmware gpio [%d]\n",
				__func__, platform_data->firm_gpio);
			goto err_irq_gpio;
>>>>>>> FETCH_HEAD
		}
		r = gpio_direction_output(platform_data->firm_gpio, 0);
		if (r) {
			dev_err(&client->dev,
<<<<<<< HEAD
			"%s: cannot set direction for gpio [%d]\n",
			__func__, platform_data->firm_gpio);
			goto err_irq;
		}
		nqx_dev->firm_gpio = platform_data->firm_gpio;
	} else {
		dev_err(&client->dev,
			"%s: firm gpio not provided\n", __func__);
=======
			"%s: cannot set direction for nfc firmware gpio [%d]\n",
			__func__, platform_data->firm_gpio);
			goto err_firm_gpio;
		}
	} else {
		dev_err(&client->dev,
			"%s: firm gpio not provided\n", __func__);
		goto err_irq_gpio;
	}
	if (gpio_is_valid(platform_data->ese_gpio)) {
		r = gpio_request(platform_data->ese_gpio,
				"nfc-ese_pwr");
		if (r) {
			nqx_dev->ese_gpio = -EINVAL;
			dev_err(&client->dev,
				"%s: unable to request nfc ese gpio [%d]\n",
					__func__, platform_data->ese_gpio);
			/* ese gpio optional so we should continue */
		} else {
			nqx_dev->ese_gpio = platform_data->ese_gpio;
			r = gpio_direction_output(platform_data->ese_gpio, 0);
			if (r) {
				/* free ese gpio and set invalid
				   to avoid further use
				*/
				gpio_free(platform_data->ese_gpio);
				nqx_dev->ese_gpio = -EINVAL;
				dev_err(&client->dev,
				"%s: cannot set direction for nfc ese gpio [%d]\n",
				__func__, platform_data->ese_gpio);
				/* ese gpio optional so we should continue */
			}
		}
	} else {
		nqx_dev->ese_gpio = -EINVAL;
		dev_err(&client->dev,
			"%s: ese gpio not provided\n", __func__);
		/* ese gpio optional so we should continue */
>>>>>>> FETCH_HEAD
	}
	if (gpio_is_valid(platform_data->clkreq_gpio)) {
		r = gpio_request(platform_data->clkreq_gpio,
			"nfc_clkreq_gpio");
		if (r) {
			dev_err(&client->dev,
<<<<<<< HEAD
				"%s: unable to request gpio [%d]\n",
				__func__, platform_data->clkreq_gpio);
			goto err_clkreq_gpio;
=======
				"%s: unable to request nfc clkreq gpio [%d]\n",
				__func__, platform_data->clkreq_gpio);
			goto err_ese_gpio;
>>>>>>> FETCH_HEAD
		}
		r = gpio_direction_input(platform_data->clkreq_gpio);
		if (r) {
			dev_err(&client->dev,
<<<<<<< HEAD
			"%s: cannot set direction for gpio [%d]\n",
			__func__, platform_data->clkreq_gpio);
			goto err_clkreq_gpio;
		}
		nqx_dev->clkreq_gpio = platform_data->clkreq_gpio;
	} else {
		dev_err(&client->dev,
			"%s: clkreq gpio not provided\n", __func__);
=======
			"%s: cannot set direction for nfc clkreq gpio [%d]\n",
			__func__, platform_data->clkreq_gpio);
			goto err_clkreq_gpio;
		}
	} else {
		dev_err(&client->dev,
			"%s: clkreq gpio not provided\n", __func__);
		goto err_ese_gpio;
>>>>>>> FETCH_HEAD
	}

	nqx_dev->en_gpio = platform_data->en_gpio;
	nqx_dev->irq_gpio = platform_data->irq_gpio;
	nqx_dev->firm_gpio  = platform_data->firm_gpio;
<<<<<<< HEAD
=======
	nqx_dev->clkreq_gpio = platform_data->clkreq_gpio;
	nqx_dev->pdata = platform_data;
>>>>>>> FETCH_HEAD

	/* init mutex and queues */
	init_waitqueue_head(&nqx_dev->read_wq);
	mutex_init(&nqx_dev->read_mutex);
	spin_lock_init(&nqx_dev->irq_enabled_lock);

	nqx_dev->nqx_device.minor = MISC_DYNAMIC_MINOR;
	nqx_dev->nqx_device.name = "nq-nci";
	nqx_dev->nqx_device.fops = &nfc_dev_fops;

	r = misc_register(&nqx_dev->nqx_device);
	if (r) {
		dev_err(&client->dev, "%s: misc_register failed\n", __func__);
		goto err_misc_register;
	}

	/* NFC_INT IRQ */
	nqx_dev->irq_enabled = true;
	r = request_irq(client->irq, nqx_dev_irq_handler,
<<<<<<< HEAD
			  IRQF_TRIGGER_RISING, client->name, nqx_dev);
=======
			  IRQF_TRIGGER_HIGH, client->name, nqx_dev);
>>>>>>> FETCH_HEAD
	if (r) {
		dev_err(&client->dev, "%s: request_irq failed\n", __func__);
		goto err_request_irq_failed;
	}
	nqx_disable_irq(nqx_dev);

<<<<<<< HEAD
	device_init_wakeup(&client->dev, true);
	device_set_wakeup_capable(&client->dev, true);
	i2c_set_clientdata(client, nqx_dev);
	gpio_set_value(platform_data->en_gpio, 1);

	dev_dbg(&client->dev,
	"%s: probing nq0 exited successfully\n",
		 __func__);
	return 0;

=======
	/*
	 * To be efficient we need to test whether nfcc hardware is physically
	 * present before attempting further hardware initialisation.
	 *
	 */
	r = nfcc_hw_check(client , platform_data->en_gpio);
	if (r) {
		/* make sure NFCC is not enabled */
		gpio_set_value(platform_data->en_gpio, 0);
		/* We don't think there is hardware switch NFC OFF */
		goto err_request_hw_check_failed;
	}

	/* Register reboot notifier here */
	r = register_reboot_notifier(&nfcc_notifier);
	if (r) {
		dev_err(&client->dev,
			"%s: cannot register reboot notifier(err = %d)\n",
			__func__, r);
		/* nfcc_hw_check function not doing memory
		   allocation so using same goto target here
		*/
		goto err_request_hw_check_failed;
	}

#ifdef NFC_KERNEL_BU
	r = nqx_clock_select(nqx_dev);
	if (r < 0) {
		dev_err(&client->dev,
			"%s: nqx_clock_select failed\n", __func__);
		goto err_clock_en_failed;
	}
	gpio_set_value(platform_data->en_gpio, 1);
#endif
	device_init_wakeup(&client->dev, true);
	device_set_wakeup_capable(&client->dev, true);
	i2c_set_clientdata(client, nqx_dev);
	nqx_dev->irq_wake_up = false;

	dev_err(&client->dev,
	"%s: probing NFCC NQxxx exited successfully\n",
		 __func__);
	return 0;

#ifdef NFC_KERNEL_BU
err_clock_en_failed:
	unregister_reboot_notifier(&nfcc_notifier);
#endif
err_request_hw_check_failed:
	free_irq(client->irq, nqx_dev);
>>>>>>> FETCH_HEAD
err_request_irq_failed:
	misc_deregister(&nqx_dev->nqx_device);
err_misc_register:
	mutex_destroy(&nqx_dev->read_mutex);
err_clkreq_gpio:
	gpio_free(platform_data->clkreq_gpio);
<<<<<<< HEAD
err_irq:
	gpio_free(platform_data->irq_gpio);
err_en_gpio:
	gpio_free(platform_data->en_gpio);
err_free_dev:
	kfree(nqx_dev);

=======
err_ese_gpio:
	/* optional gpio, not sure was configured in probe */
	if (nqx_dev->ese_gpio > 0)
		gpio_free(platform_data->ese_gpio);
err_firm_gpio:
	gpio_free(platform_data->firm_gpio);
err_irq_gpio:
	gpio_free(platform_data->irq_gpio);
err_en_gpio:
	gpio_free(platform_data->en_gpio);
err_mem:
	kfree(nqx_dev->kbuf);
err_free_dev:
	kfree(nqx_dev);
err_free_data:
	if (client->dev.of_node)
		devm_kfree(&client->dev, platform_data);
err_platform_data:
	dev_err(&client->dev,
	"%s: probing nqxx failed, check hardware\n",
		 __func__);
>>>>>>> FETCH_HEAD
	return r;
}

static int nqx_remove(struct i2c_client *client)
{
<<<<<<< HEAD
	struct nqx_dev *nqx_dev;

	nqx_dev = i2c_get_clientdata(client);
	free_irq(client->irq, nqx_dev);
	misc_deregister(&nqx_dev->nqx_device);
	mutex_destroy(&nqx_dev->read_mutex);
	gpio_free(nqx_dev->irq_gpio);
	gpio_free(nqx_dev->en_gpio);

	kfree(nqx_dev);
	return 0;
=======
	int ret = 0;
	struct nqx_dev *nqx_dev;

	nqx_dev = i2c_get_clientdata(client);
	if (!nqx_dev) {
		dev_err(&client->dev,
		"%s: device doesn't exist anymore\n", __func__);
		ret = -ENODEV;
		goto err;
	}

	unregister_reboot_notifier(&nfcc_notifier);
	free_irq(client->irq, nqx_dev);
	misc_deregister(&nqx_dev->nqx_device);
	mutex_destroy(&nqx_dev->read_mutex);
	gpio_free(nqx_dev->clkreq_gpio);
	/* optional gpio, not sure was configured in probe */
	if (nqx_dev->ese_gpio > 0)
		gpio_free(nqx_dev->ese_gpio);
	gpio_free(nqx_dev->firm_gpio);
	gpio_free(nqx_dev->irq_gpio);
	gpio_free(nqx_dev->en_gpio);
	kfree(nqx_dev->kbuf);
	if (client->dev.of_node)
		devm_kfree(&client->dev, nqx_dev->pdata);

	kfree(nqx_dev);
err:
	return ret;
>>>>>>> FETCH_HEAD
}

static int nqx_suspend(struct device *device)
{
	struct i2c_client *client = to_i2c_client(device);
<<<<<<< HEAD

	if (device_may_wakeup(&client->dev))
		enable_irq_wake(client->irq);
=======
	struct nqx_dev *nqx_dev = i2c_get_clientdata(client);

	if (device_may_wakeup(&client->dev) && nqx_dev->irq_enabled) {
		if (!enable_irq_wake(client->irq))
			nqx_dev->irq_wake_up = true;
	}
>>>>>>> FETCH_HEAD
	return 0;
}

static int nqx_resume(struct device *device)
{
	struct i2c_client *client = to_i2c_client(device);
<<<<<<< HEAD

	if (device_may_wakeup(&client->dev))
		disable_irq_wake(client->irq);
=======
	struct nqx_dev *nqx_dev = i2c_get_clientdata(client);

	if (device_may_wakeup(&client->dev) && nqx_dev->irq_wake_up) {
		if (!disable_irq_wake(client->irq))
			nqx_dev->irq_wake_up = false;
	}
>>>>>>> FETCH_HEAD
	return 0;
}

static const struct i2c_device_id nqx_id[] = {
	{"nqx-i2c", 0},
	{}
};

static const struct dev_pm_ops nfc_pm_ops = {
	SET_SYSTEM_SLEEP_PM_OPS(nqx_suspend, nqx_resume)
};

static struct i2c_driver nqx = {
	.id_table = nqx_id,
	.probe = nqx_probe,
	.remove = nqx_remove,
	.driver = {
		.owner = THIS_MODULE,
		.name = "nq-nci",
		.of_match_table = msm_match_table,
		.pm = &nfc_pm_ops,
	},
};


static int nfcc_reboot(struct notifier_block *notifier, unsigned long val,
			  void *v)
{
	gpio_set_value(disable_ctrl, 1);
	return NOTIFY_OK;
}

/*
 * module load/unload record keeping
 */
static int __init nqx_dev_init(void)
{
	return i2c_add_driver(&nqx);
}
module_init(nqx_dev_init);

static void __exit nqx_dev_exit(void)
{
	unregister_reboot_notifier(&nfcc_notifier);
	i2c_del_driver(&nqx);
}
module_exit(nqx_dev_exit);

MODULE_DESCRIPTION("NFC nqx");
MODULE_LICENSE("GPL v2");

